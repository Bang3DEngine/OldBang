#include "FileTracker.h"

#include <QFileInfo>
#include <QDateTime>

#include "Bang/IO.h"
#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Asset.h"
#include "Bang/XMLParser.h"
#include "Bang/AssetsManager.h"

FileTracker::FileTracker()
{
    m_refreshTimer.start(3000);
    QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                     this, SLOT(Refresh()));
}

void FileTracker::Refresh()
{
    EpochTime timeBeforeRefreshing = QDateTime::currentMSecsSinceEpoch();

    List <String> allFiles = IO::GetFiles(IO::GetProjectAssetsRootAbs(), true);

    // Refresh files modification date and dependencies
    for (const String& absFilepath : allFiles)
    {
        bool isNewFile = !m_fileChangeTimes.ContainsKey(absFilepath);

        RefreshFileModificationDate(absFilepath);
        if (isNewFile || HasChanged(absFilepath))
        {
            RefreshFileDependencies(absFilepath);
        }
    }

    // Check if some file has changed, and if so, notify the engine properly
    // Do this twice, to avoid cases in which file dependencies update after
    ReloadChangedFiles(allFiles);
    ReloadChangedFiles(allFiles);

    // Mark them as seen later, not before, because we need to be aware of
    // changed files through the whole refresh process above, so no mark there.
    for (const String& absFilepath : allFiles)
    {
        m_fileSeenTimes.Set(absFilepath, timeBeforeRefreshing);
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

void FileTracker::ReloadChangedFiles(const List<String>& allFiles) const
{
    for (const String& absFilepath : allFiles)
    {
        File file(absFilepath); // Treat file in case it has changed
        if (file.IsAsset() && HasChanged(absFilepath))
        {
          Debug_Log("Reloading asset: " << absFilepath);
          AssetsManager::ReloadAsset(absFilepath);
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
    if (changedTime > seenTime) { Debug_Log("HasChanged: " << absFilepath); }
    return changedTime > seenTime;
}
