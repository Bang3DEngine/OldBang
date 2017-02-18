#include "FileReferencesManager.h"

#include "File.h"
#include "Debug.h"
#include "XMLNode.h"
#include "Explorer.h"
#include "IFileable.h"
#include "FileWriter.h"
#include "Persistence.h"

FileReferencesManager::FileReferencesManager()
{
}

FileReferencesManager::~FileReferencesManager()
{

}

void FileReferencesManager::RegisterIFileable(IFileable *fileable)
{
    m_inMemoryFileables.insert(fileable);
}

void FileReferencesManager::UnRegisterIFileable(IFileable *fileable)
{
    m_inMemoryFileables.erase(fileable);
}

void FileReferencesManager::OnFileOrDirNameAboutToBeChanged(
                                            const String &absFilepathBefore,
                                            const String &absFilepathNow)
{
    ASSERT(!absFilepathBefore.Empty()); ASSERT(!absFilepathNow.Empty());
    m_queuedNameChanges.push( std::make_pair(absFilepathBefore,
                                             absFilepathNow));
    QTimer::singleShot(100, this, SLOT(TreatNextQueuedFileOrDirNameChange()));
}

void FileReferencesManager::TreatNextQueuedFileOrDirNameChange()
{
    const std::pair<String,String> &fileNamesPair = m_queuedNameChanges.front();
    String absFilepathBefore = fileNamesPair.first;
    String absFilepathNow    = fileNamesPair.second;
    m_queuedNameChanges.pop();

    bool fileHasMoved = !absFilepathBefore.Empty() &&
                        !Persistence::Exists(absFilepathBefore) &&
                         Persistence::Exists(absFilepathNow);

    if (fileHasMoved) // Sometimes it triggers drop but nothing is moved
    {
        String relPathBefore = Persistence::ToRelative(absFilepathBefore,false);
        String relPathNow    = Persistence::ToRelative(absFilepathNow, false);
        if ( Persistence::IsDir(absFilepathNow) )
        {
            relPathBefore += "/";
            relPathNow += "/";
        }
        RefactorFiles(relPathBefore, relPathNow);
        RefactorIFileables(relPathBefore, relPathNow);
    }
}

FileReferencesManager *FileReferencesManager::GetInstance()
{
    return Explorer::GetInstance()->m_fileRefsManager;
}

void FileReferencesManager::RefactorFiles(const String &relPathBefore,
                                          const String &relPathNow)
{
    List<String> allFiles =
          Persistence::GetFiles(Persistence::GetProjectAssetsRootAbs(), true);
    for (const String &filepath : allFiles)
    {
        File f(filepath);
        if (!f.IsAsset()) { continue; }

        String fileXMLContents = f.GetContents();
        int replacements = fileXMLContents.Replace(relPathBefore, relPathNow);
        if (replacements > 0)
        {
            FileWriter::WriteToFile(filepath, fileXMLContents);
        }
    }
}

void FileReferencesManager::RefactorIFileables(const String &relPathBefore,
                                               const String &relPathNow)
{
    for (IFileable *fileable : m_inMemoryFileables)
    {
        // First get its xml description and try to replace the old path with
        // the new path
        String xmlInfoStr = fileable->GetXMLInfoString();
        int replacements = xmlInfoStr.Replace(relPathBefore, relPathNow);

        // If some replacement has been done, tell it to refresh itself with
        // the new refactored info.
        if (replacements > 0)
        {
            XMLNode *newXMLInfo = XMLNode::FromString(xmlInfoStr);
            fileable->ReadXMLInfo(newXMLInfo);
            delete newXMLInfo;
        }
    }
}

