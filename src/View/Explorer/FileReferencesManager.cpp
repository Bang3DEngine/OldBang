#include "FileReferencesManager.h"

#include "Map.h"
#include "File.h"
#include "Debug.h"
#include "XMLNode.h"
#include "Explorer.h"
#include "IO.h"
#include "XMLAttribute.h"
#include "SerializableObject.h"

FileReferencesManager::FileReferencesManager()
{
}

FileReferencesManager::~FileReferencesManager()
{

}

void FileReferencesManager::RegisterSerializableObject(SerializableObject *fileable)
{
    m_inMemoryFileables.Insert(fileable);
}

void FileReferencesManager::UnRegisterSerializableObject(SerializableObject *fileable)
{
    m_inMemoryFileables.Remove(fileable);
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
                        !IO::Exists(absFilepathBefore) &&
                         IO::Exists(absFilepathNow);

    if (fileHasMoved) // Sometimes it triggers drop but nothing is moved, check
    {
        String relPathBefore = IO::ToRelative(absFilepathBefore,false);
        String relPathNow    = IO::ToRelative(absFilepathNow, false);
        if ( IO::IsDir(absFilepathNow) )
        {
            relPathBefore += "/";
            relPathNow += "/";
        }
        RefactorFiles(relPathBefore, relPathNow);
        RefactorSerializableObject(relPathBefore, relPathNow);
    }
}

FileReferencesManager *FileReferencesManager::GetInstance()
{
    Explorer *exp = Explorer::GetInstance();
    return exp ? exp->m_fileRefsManager : nullptr;
}

void FileReferencesManager::RefactorFiles(const String &relPathBefore,
                                          const String &relPathNow)
{
    List<String> allFiles =
          IO::GetFiles(IO::GetProjectAssetsRootAbs(), true);
    for (const String &filepath : allFiles)
    {
        File f(filepath);
        if (!f.IsAsset()) { continue; }

        String fileXMLContents = f.GetContents();
        XMLNode *auxXMLInfo = XMLNode::FromString(fileXMLContents);
        if (auxXMLInfo && RefactorXMLInfo(auxXMLInfo, relPathBefore, relPathNow))
        {
            IO::WriteToFile(filepath, auxXMLInfo->ToString());
        }

        if (auxXMLInfo) { delete auxXMLInfo; }
    }
}

void FileReferencesManager::RefactorSerializableObject(const String &relPathBefore,
                                               const String &relPathNow)
{
    for (SerializableObject *fileable : m_inMemoryFileables)
    {
        // First get its xml description, create an XMLNode, pass it to the
        // refactoring function, let the fileable read it to be updated, and
        // delete the created aux XMLNode
        String xmlInfoStr = fileable->GetSerializedString();
        XMLNode *auxXMLInfo = XMLNode::FromString(xmlInfoStr);
        if (auxXMLInfo && RefactorXMLInfo(auxXMLInfo, relPathBefore, relPathNow))
        {
            fileable->Read(*auxXMLInfo);
        }
        if (auxXMLInfo) { delete auxXMLInfo; }
    }
}

bool FileReferencesManager::RefactorXMLInfo(XMLNode *xmlInfo,
                                            const String &relPathBefore,
                                            const String &relPathNow)
{
    bool hasBeenModified = false;

    // Refactor all its file attributes that are non-engine
    const Map<String, XMLAttribute> &xmlAttributes = xmlInfo->GetAttributes();
    for (const std::pair<String, XMLAttribute> &name_attr : xmlAttributes)
    {
        XMLAttribute attr = name_attr.second;
        if (attr.GetType() != XMLAttribute::Type::File) { continue; }
        if (attr.HasProperty(XMLProperty::IsEngineFile)) { continue; }

        String relFilepath = attr.GetValue();
        //Debug_Log(relFilepath << ", " << relPathBefore);
        int numReplacements = relFilepath.ReplaceInSitu(relPathBefore, relPathNow);
        if (numReplacements > 0)
        {
          //  Debug_Log("Refactored in " << relFilepath << " attr " <<
          //            name_attr.first << ": " << relPathBefore <<
          //            " by " << relPathNow);
            attr.SetValue(relFilepath);
            xmlInfo->SetAttribute(attr);
            hasBeenModified = true;
        }
    }

    // Now refactor its children
    List<XMLNode*> xmlChildren = xmlInfo->GetChildren();
    for (XMLNode *xmlChild : xmlChildren)
    {
        hasBeenModified = hasBeenModified ||
                          RefactorXMLInfo(xmlChild, relPathBefore, relPathNow);
    }

    return hasBeenModified;
}

