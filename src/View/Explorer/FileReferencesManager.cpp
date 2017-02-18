#include "FileReferencesManager.h"

#include "Map.h"
#include "File.h"
#include "Debug.h"
#include "XMLNode.h"
#include "Explorer.h"
#include "IFileable.h"
#include "FileWriter.h"
#include "Persistence.h"
#include "XMLAttribute.h"

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
        XMLNode *auxXMLInfo = XMLNode::FromString(fileXMLContents);
        RefactorXMLInfo(auxXMLInfo, relPathBefore, relPathNow);

        FileWriter::WriteToFile(filepath, auxXMLInfo->ToString());

        delete auxXMLInfo;
    }
}

void FileReferencesManager::RefactorIFileables(const String &relPathBefore,
                                               const String &relPathNow)
{
    for (IFileable *fileable : m_inMemoryFileables)
    {
        // First get its xml description, create an XMLNode, pass it to the
        // refactoring function, let the fileable read it to be updated, and
        // delete the created aux XMLNode
        String xmlInfoStr = fileable->GetXMLInfoString();
        XMLNode *auxXMLInfo = XMLNode::FromString(xmlInfoStr);
        RefactorXMLInfo(auxXMLInfo, relPathBefore, relPathNow);

        fileable->ReadXMLInfo(auxXMLInfo);

        delete auxXMLInfo;
    }
}

void FileReferencesManager::RefactorXMLInfo(XMLNode *xmlInfo,
                                            const String &relPathBefore,
                                            const String &relPathNow)
{
    // Refactor all its file attributes that are non-engine
    const Map<String, XMLAttribute> &xmlAttributes = xmlInfo->GetAttributes();
    for (const std::pair<String, XMLAttribute> &name_attr : xmlAttributes)
    {
        XMLAttribute attr = name_attr.second;
        if (attr.GetType() != XMLAttribute::Type::File) { continue; }
        if (!attr.HasProperty(XMLProperty::IsEngineFile)) { continue; }

        String filepath = attr.GetFilepath();
        if (filepath.Replace(relPathBefore, relPathNow) > 0)
        {
            attr.SetValue(filepath);
            xmlInfo->SetAttribute(attr);
        }
    }

    // Now refactor its children
    List<XMLNode*> xmlChildren = xmlInfo->GetChildren();
    for (XMLNode *xmlChild : xmlChildren)
    {
        RefactorXMLInfo(xmlChild, relPathBefore, relPathNow);
    }
}

