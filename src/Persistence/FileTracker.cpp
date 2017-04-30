#include "FileTracker.h"

#include <QFileInfo>
#include <QDateTime>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Asset.h"
#include "Bang/XMLParser.h"
#include "Bang/AssetsManager.h"

FileTracker::FileTracker()
{
    m_timeBeforeRefreshing = Time::GetNow();

    m_refreshTimer.setInterval(c_refreshTime);
    m_refreshTimer.moveToThread(&m_refreshThread);
    QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                     this, SLOT(Refresh()), Qt::DirectConnection);
    m_refreshTimer.connect(&m_refreshThread, SIGNAL(started()), SLOT(start()));
    m_refreshThread.start();

    QObject::connect(this, SIGNAL(ChangedFilesRefreshed()),
                     this, SLOT(ReloadChangedFiles()));
}

FileTracker *FileTracker::GetInstance()
{
    return Application::GetInstance()->m_fileTracker;
}

List< std::pair<String, String> > FileTracker::GetMovedPathsList() const
{
    List<String> oldPathsList = m_lastPathsList;
    m_lastPathsList = GetPathsToTrack();

    const List<String> &newPathsList = m_lastPathsList;

    List< std::pair<String,String> > result;
    for (const String& oldPath : oldPathsList)
    {
        if (newPathsList.Contains(oldPath)) { continue; } // Not moved

        // This old path is missing now...where has it gone?
        // Search it in the new files list
        String oldPathBaseName = IO::GetBaseName(oldPath);
        for (const String &newPath : newPathsList)
        {
            String newPathBaseName = IO::GetBaseName(newPath);
            if (oldPathBaseName != newPathBaseName) { continue; }
            if (oldPathsList.Contains(newPath)) { continue; }

            // This file is the older file moved. Because it has the same
            // base name, and did not exist before. Register as renamed/moved.
            result.PushBack( std::make_pair(oldPath, newPath) );
        }
    }
    return result;
}

void FileTracker::TrackFilesWithExtension(const String &extension)
{
    FileTracker *ft = FileTracker::GetInstance();
    ft->m_additionalExtensions.Remove(extension);
    ft->m_additionalExtensions.Add(extension);
}

bool FileTracker::HasFileChanged(const String &absFilepath, EpochTime since)
{
    FileTracker *ft = FileTracker::GetInstance();

    if (!ft->m_lastModifTimes.ContainsKey(absFilepath)) { return true; }

    EpochTime lastChangeTime = ft->m_lastModifTimes.Get(absFilepath);
    return FileTracker::HasFileChanged(since, lastChangeTime);
}

void FileTracker::Refresh()
{
    m_timeBeforeRefreshing = Time::GetNow();

    List<String> allFiles = GetPathsToTrack();

    // Refresh files modification date and dependencies
    bool someFileHasChanged = false;
    for (const String& absFilepath : allFiles)
    {
        RefreshFileModificationDate(absFilepath);

        bool isNewFile = !m_lastModifTimes.ContainsKey(absFilepath);
        if (isNewFile || HasFileChanged(absFilepath))
        {
            someFileHasChanged = true;
            RefreshFileDependencies(absFilepath);
        }
    }

    if (someFileHasChanged)
    {
        emit ChangedFilesRefreshed(); // ReloadChangedFiles()
    }
}

void FileTracker::ReloadChangedFiles()
{
    // Reload the files that have changed from the last time

    List<String> allFiles = GetPathsToTrack();
    for (const String& absFilepath : allFiles)
    {
        File file(absFilepath); // Treat file in case it has changed
        if (MustTrackFile(file) && HasFileChanged(absFilepath))
        {
            AssetsManager::ReloadAsset(absFilepath);
        }
    }

    // Mark them as seen later, not before, because we need to be aware of
    // changed files through the whole refresh process above, so no mark there.
    for (const String& absFilepath : allFiles)
    {
        m_lastSeenTimes.Set(absFilepath, m_timeBeforeRefreshing);
    }
}

List<String> FileTracker::GetPathsToTrack() const
{
    const String& engineRoot = IO::GetEngineAssetsRootAbs();
    const String& assetsRoot = IO::GetProjectAssetsRootAbs();

    List<String> files;
    files = files.Concat(IO::GetFiles(assetsRoot, true));
    files = files.Concat(IO::GetSubDirectories(assetsRoot, true));
    files = files.Concat(IO::GetFiles(engineRoot, true, m_additionalExtensions));
    files = files.Concat({assetsRoot});

    return files;
}

bool FileTracker::MustTrackFile(const File &file) const
{
    return file.IsFile() &&
           (
                file.IsAsset() ||
                m_additionalExtensions.Contains( file.GetExtension() )
           );
}

void FileTracker::RefreshFileModificationDate(const String& absFilepath)
{
    // Update modification time
    QFileInfo fInfo(absFilepath.ToQString());
    EpochTime modTime = fInfo.lastModified().toMSecsSinceEpoch();

    // If it is a new file, register it.
    if (!m_lastModifTimes.ContainsKey(absFilepath))
    {
        EpochTime currentEpoch = Time::GetNow();
        m_lastSeenTimes.Set(absFilepath, currentEpoch);
        m_fileDependencies.Set(absFilepath, List<String>());
    }
    m_lastModifTimes.Set(absFilepath, modTime);
}

void FileTracker::RefreshFileDependencies(const String &absFilepath)
{
    // If its an asset (XML), then add the file dependencies by
    // examining the file contents, and finding all the attributes which are
    // filepaths.
    File file(absFilepath);
    ENSURE ( MustTrackFile(file) );

    List<String>& depsList = m_fileDependencies.Get(absFilepath);
    depsList.Clear();

    XMLNode xmlInfo = XMLParser::FromFile(absFilepath);
    List< std::pair<String, XMLAttribute> > attrs =
                                           xmlInfo.GetAttributesListInOrder();
    for (const std::pair<String, XMLAttribute>& attrPair : attrs)
    {
        const XMLAttribute& attr = attrPair.second;
        if (attr.GetType() == XMLAttribute::Type::File)
        {
            const String& depFilepath = attr.GetFilepath();
            if (IO::ExistsFile(depFilepath))
            {
                depsList.Add(depFilepath);
            }
        }
    }
}

bool FileTracker::HasFileChanged(const String &absFilepath) const
{
    Set<String> alreadyCheckedFiles;
    return HasFileChanged(absFilepath, alreadyCheckedFiles);
}

bool FileTracker::HasFileChanged(const String& absFilepath,
                                 Set<String>& alreadyCheckedFiles) const
{
    if (!m_lastModifTimes.ContainsKey(absFilepath)) { return false; }
    if (!m_lastSeenTimes.ContainsKey(absFilepath)) { return false; }

    if (!alreadyCheckedFiles.Contains(absFilepath))
    {
        alreadyCheckedFiles.Insert(absFilepath);
        for (const String& depAbsFilepath : m_fileDependencies.Get(absFilepath))
        {
            if (HasFileChanged(depAbsFilepath, alreadyCheckedFiles)) { return true; }
        }
    }

    EpochTime changedTime = m_lastModifTimes.Get(absFilepath);
    EpochTime seenTime    = m_lastSeenTimes.Get(absFilepath);
    return FileTracker::HasFileChanged(seenTime, changedTime);
}

bool FileTracker::HasFileChanged(EpochTime lastSeenTime,
                                 EpochTime modificationTime)
{
    // Must compare like this because it can happen that the modification
    // times given by the system are not in milis, but in seconds.

    // So, this is a bit conservative comparison, we dont want to miss any file
    // changing.
    return modificationTime >= lastSeenTime - c_refreshTime * 1.5;
}
