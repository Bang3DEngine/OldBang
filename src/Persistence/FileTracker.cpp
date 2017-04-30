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

List< std::pair<Path, Path> > FileTracker::GetMovedPathsList() const
{
    List<Path> oldPathsList = m_lastPathsList;
    m_lastPathsList = GetPathsToTrack();

    const List<Path> &newPathsList = m_lastPathsList;

    List< std::pair<Path,Path> > result;
    for (const Path& oldPath : oldPathsList)
    {
        if (newPathsList.Contains(oldPath)) { continue; } // Not moved

        // This old path is missing now...where has it gone?
        // Search it in the new files list
        String oldPathName = oldPath.GetNameExt();
        for (const Path &newPath : newPathsList)
        {
            String newPathName = newPath.GetNameExt();
            if (oldPathName != newPathName) { continue; }
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

bool FileTracker::HasFileChanged(const Path &filepath, EpochTime since)
{
    FileTracker *ft = FileTracker::GetInstance();

    if (!ft->m_lastModifTimes.ContainsKey(filepath)) { return true; }

    EpochTime lastChangeTime = ft->m_lastModifTimes.Get(filepath);
    return FileTracker::HasFileChanged(since, lastChangeTime);
}

void FileTracker::Refresh()
{
    m_timeBeforeRefreshing = Time::GetNow();
    if (m_fileDependencies.Empty()) // Initial catch of values & dependencies
    {
        List<Path> paths = GetPathsToTrack();
        for (const Path& path : paths)
        {
            RefreshFileModificationDate(path);
            RefreshFileDependencies(path);
        }
    }

    // Refresh files modification date and dependencies
    List<Path> allFiles = GetPathsToTrack();
    bool someFileHasChanged = false;
    for (const Path& filepath : allFiles)
    {
        RefreshFileModificationDate(filepath);
        if (HasFileChanged(filepath))
        {
            someFileHasChanged = true;
            RefreshFileDependencies(filepath);
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

    List<Path> allPaths = GetPathsToTrack();
    for (const Path& filepath : allPaths)
    {
        File file(filepath); // Treat file in case it has changed
        if (MustTrackFile(file) && HasFileChanged(filepath))
        {
            AssetsManager::ReloadAsset(filepath);
        }
    }

    // Mark them as seen later, not before, because we need to be aware of
    // changed files through the whole refresh process above, so no mark there.
    for (const Path& filepath : allPaths)
    {
        m_lastSeenTimes.Set(filepath, m_timeBeforeRefreshing);
    }
}

List<Path> FileTracker::GetPathsToTrack() const
{
    Path engineRoot(IO::GetEngineAssetsRootAbs());
    Path assetsRoot(IO::GetProjectAssetsRootAbs());

    List<Path> files;
    files = files.Concat(assetsRoot.GetFiles(true));
    files = files.Concat(assetsRoot.GetSubDirectories(true));
    files = files.Concat(engineRoot.GetFiles(true, m_additionalExtensions));
    files = files.Concat({assetsRoot});

    return files;
}

bool FileTracker::MustTrackFile(const File &file) const
{
    return file.GetPath().IsFile() &&
           (
                file.IsAsset() ||
                m_additionalExtensions.Contains(file.GetPath().GetExtension())
           );
}

void FileTracker::RefreshFileModificationDate(const Path &filepath)
{
    // Update modification time
    QFileInfo fInfo(filepath.GetAbsolute().ToQString());
    EpochTime modTime = fInfo.lastModified().toMSecsSinceEpoch();

    // If it is a new file, register it.
    if (!m_lastModifTimes.ContainsKey(filepath))
    {
        EpochTime currentEpoch = Time::GetNow();
        m_lastSeenTimes.Set(filepath, currentEpoch);
        m_fileDependencies.Set(filepath, List<Path>());
    }
    m_lastModifTimes.Set(filepath, modTime);
}

void FileTracker::RefreshFileDependencies(const Path &filepath)
{
    // If its an asset (XML), then add the file dependencies by
    // examining the file contents, and finding all the attributes which are
    // filepaths.
    File file(filepath);
    ENSURE ( MustTrackFile(file) );

    List<Path>& depsList = m_fileDependencies.Get(filepath);
    depsList.Clear();

    XMLNode xmlInfo = XMLParser::FromFile(filepath);
    List< std::pair<String, XMLAttribute> > attrs =
                                           xmlInfo.GetAttributesListInOrder();
    for (const std::pair<String, XMLAttribute>& attrPair : attrs)
    {
        const XMLAttribute& attr = attrPair.second;
        if (attr.GetType() == XMLAttribute::Type::File)
        {
            Path depFilepath = attr.GetFilepath();
            if (depFilepath.Exists()) { depsList.Add(depFilepath); }
        }
    }
}

bool FileTracker::HasFileChanged(const Path &absFilepath) const
{
    Set<Path> alreadyCheckedFiles;
    return HasFileChanged(absFilepath, alreadyCheckedFiles);
}

bool FileTracker::HasFileChanged(const Path& filepath,
                                 Set<Path>& alreadyCheckedFiles) const
{
    if (!m_lastModifTimes.ContainsKey(filepath)) { return false; }
    if (!m_lastSeenTimes.ContainsKey(filepath))  { return false; }

    if (!alreadyCheckedFiles.Contains(filepath))
    {
        alreadyCheckedFiles.Insert(filepath);
        for (const Path& depFilepath : m_fileDependencies.Get(filepath))
        {
            if (HasFileChanged(depFilepath, alreadyCheckedFiles))
            {
                return true;
            }
        }
    }

    EpochTime changedTime = m_lastModifTimes.Get(filepath);
    EpochTime seenTime    = m_lastSeenTimes.Get(filepath);
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
