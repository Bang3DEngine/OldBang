#include "FileTracker.h"

#include <QFileInfo>
#include <QDateTime>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/File.h"
#include "Bang/Asset.h"
#include "Bang/XMLParser.h"
#include "Bang/AssetsManager.h"

FileTracker::FileTracker()
{
    m_timeBeforeRefreshing = QDateTime::currentMSecsSinceEpoch();

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
    List<String> oldPathsList = m_assetsPathsList;

    RefreshAssetPathsList();
    const List<String> &newPathsList = m_assetsPathsList;

    List< std::pair<String,String> > result;
    for (const String& oldPath : oldPathsList)
    {
        if (newPathsList.Contains(oldPath)) { continue; } // Not moved

        // This old path is missing now...where has it gone?
        // Search it in the new files list
        String oldPathBaseName = IO::GetFileName(oldPath);
        for (const String &newPath : newPathsList)
        {
            String newPathBaseName = IO::GetFileName(newPath);
            if (oldPathBaseName != newPathBaseName) { continue; }
            if (oldPathsList.Contains(newPath)) { continue; }

            // This file is the older file moved. Because it has the same
            // base name, and did not exist before. Register as renamed/moved.
            result.PushBack( std::make_pair(oldPath, newPath) );
        }
    }
    return result;
}

void FileTracker::Refresh()
{
    m_timeBeforeRefreshing = QDateTime::currentMSecsSinceEpoch();

    List <String> allFiles = IO::GetFiles(IO::GetProjectAssetsRootAbs(), true);

    // Refresh files modification date and dependencies
    bool someFileHasChanged = false;
    for (const String& absFilepath : allFiles)
    {
        bool isNewFile = !m_fileChangeTimes.ContainsKey(absFilepath);

        RefreshFileModificationDate(absFilepath);
        if (isNewFile || HasChanged(absFilepath))
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
    List <String> allFiles = IO::GetFiles(IO::GetProjectAssetsRootAbs(), true);
    for (const String& absFilepath : allFiles)
    {
        File file(absFilepath); // Treat file in case it has changed
        if (file.IsAsset() && HasChanged(absFilepath))
        {
            AssetsManager::ReloadAsset(absFilepath);
        }
    }

    // Mark them as seen later, not before, because we need to be aware of
    // changed files through the whole refresh process above, so no mark there.
    for (const String& absFilepath : allFiles)
    {
        m_fileSeenTimes.Set(absFilepath, m_timeBeforeRefreshing);
    }
}

void FileTracker::RefreshFileModificationDate(const String& absFilepath)
{
    // Update modification time
    QFileInfo fInfo(absFilepath.ToQString());
    EpochTime modTime = fInfo.lastModified().toMSecsSinceEpoch();

    // If it is a new file, register it.
    if (!m_fileChangeTimes.ContainsKey(absFilepath))
    {
        EpochTime currentEpoch = QDateTime::currentMSecsSinceEpoch();
        m_fileSeenTimes.Set(absFilepath, currentEpoch);
        m_fileDependencies.Set(absFilepath, List<String>());
    }
    m_fileChangeTimes.Set(absFilepath, modTime);
}

void FileTracker::RefreshFileDependencies(const String &absFilepath)
{
    // If its an asset (XML), then add the file dependencies by
    // examining the file contents, and finding all the attributes which are
    // filepaths.
    File file(absFilepath);
    if ( file.IsAsset() )
    {
        List<String>& depsList = m_fileDependencies.Get(absFilepath);
        depsList.Clear();

        XMLNode *xmlInfo = XMLParser::FromFile(absFilepath);
        if (xmlInfo)
        {
            List< std::pair<String, XMLAttribute> > attrs =
                    xmlInfo->GetAttributesListInOrder();
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
            delete xmlInfo;
        }
    }
}

bool FileTracker::HasChanged(const String &absFilepath) const
{
    Set<String> alreadyCheckedFiles;
    return HasChanged(absFilepath, alreadyCheckedFiles);
}

bool FileTracker::HasChanged(const String& absFilepath,
                             Set<String>& alreadyCheckedFiles) const
{
    if (!m_fileChangeTimes.ContainsKey(absFilepath)) { return false; }
    if (!m_fileSeenTimes.ContainsKey(absFilepath)) { return false; }

    if (!alreadyCheckedFiles.Contains(absFilepath))
    {
        alreadyCheckedFiles.Insert(absFilepath);
        for (const String& depAbsFilepath : m_fileDependencies.Get(absFilepath))
        {
            if (HasChanged(depAbsFilepath, alreadyCheckedFiles)) { return true; }
        }
    }

    EpochTime changedTime = m_fileChangeTimes.Get(absFilepath);
    EpochTime seenTime    = m_fileSeenTimes.Get(absFilepath);
    return changedTime > seenTime;
}

void FileTracker::RefreshAssetPathsList() const
{
    const String& assetsPath = IO::GetProjectAssetsRootAbs();
    List<String> files = IO::GetFiles(assetsPath, true);
    List<String> dirs = IO::GetSubDirectories(assetsPath, true);

    m_assetsPathsList = files.Concat(dirs);
}
