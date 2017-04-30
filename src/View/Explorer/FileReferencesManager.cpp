#include "Bang/FileReferencesManager.h"

#include <QTimer>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Map.h"
#include "Bang/File.h"
#include "Bang/Debug.h"
#include "FileTracker.h"
#include "Bang/XMLNode.h"
#include "Bang/Explorer.h"
#include "Bang/XMLAttribute.h"
#include "Bang/SerializableObject.h"

FileReferencesManager::FileReferencesManager()
{
    m_timer.start(5000);
    QObject::connect(&m_timer, SIGNAL(timeout()),
                     this, SLOT(CheckForMovedFiles()));
}

FileReferencesManager::~FileReferencesManager()
{

}

void FileReferencesManager::RegisterSerializableObject(SerializableObject *fileable)
{
    m_inMemorySerialObjects.Insert(fileable);
}

void FileReferencesManager::UnRegisterSerializableObject(SerializableObject *fileable)
{
    m_inMemorySerialObjects.Remove(fileable);
}

void FileReferencesManager::OnFileOrDirNameMoved(const String &absFilepathBefore,
                                                 const String &absFilepathNow)
{
    ENSURE(!absFilepathBefore.Empty());
    // ENSURE(!absFilepathNow.Empty());

    bool fileHasMoved = !absFilepathBefore.Empty() &&
                        !IO::Exists(absFilepathBefore) &&
                         IO::Exists(absFilepathNow);

    if (fileHasMoved)
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

void FileReferencesManager::CheckForMovedFiles()
{
    FileReferencesManager *frm = FileReferencesManager::GetInstance();

    List< std::pair<Path, Path> > movedPathsList =
            FileTracker::GetInstance()->GetMovedPathsList();

    for (const std::pair<Path,Path>& oldNewPath : movedPathsList)
    {
        const Path &oldPath = oldNewPath.first;
        const Path &newPath = oldNewPath.second;
        frm->OnFileOrDirNameMoved(oldPath.GetAbsolute(), newPath.GetAbsolute());
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
        XMLNode auxXMLInfo = XMLNode::FromString(fileXMLContents);
        if (RefactorXMLInfo(&auxXMLInfo, relPathBefore, relPathNow, true))
        {
            IO::WriteToFile(filepath, auxXMLInfo.ToString());
        }
    }
}

void FileReferencesManager::RefactorSerializableObject(const String &relPathBefore,
                                                       const String &relPathNow)
{
    for (SerializableObject *serialObject : m_inMemorySerialObjects)
    {
        // First get its xml description, create an XMLNode, pass it to the
        // refactoring function, let the fileable read it to be updated, and
        // delete the created aux XMLNode
        String xmlInfoStr = serialObject->GetSerializedString();
        XMLNode auxXMLInfo = XMLNode::FromString(xmlInfoStr);
        if (RefactorXMLInfo(&auxXMLInfo, relPathBefore, relPathNow, false))
        {
            serialObject->Read(auxXMLInfo);
        }
    }
}

bool FileReferencesManager::RefactorXMLInfo(XMLNode *xmlInfo,
                                            const String &relPathBefore,
                                            const String &relPathNow,
                                            bool refactorXMLChildren)
{
    bool hasBeenModified = false;

    // Refactor all its file attributes that are non-engine
    const Map<String, XMLAttribute> &xmlAttributes = xmlInfo->GetAttributes();
    for (const std::pair<String, XMLAttribute> &name_attr : xmlAttributes)
    {
        XMLAttribute attr = name_attr.second;
        if (attr.GetType() != XMLAttribute::Type::File)  { continue; }
        if (attr.HasProperty(XMLProperty::IsEngineFile)) { continue; }

        String relFilepath = attr.GetValue();
        int numReplacements = relFilepath.ReplaceInSitu(relPathBefore, relPathNow);
        if (numReplacements > 0)
        {
            XMLAttribute *attrPointer = xmlInfo->GetAttribute(attr.GetName());
            attrPointer->SetValue(relFilepath);
            hasBeenModified = true;
        }
    }

    if (refactorXMLChildren)
    {
        List<XMLNode>& xmlChildren = xmlInfo->GetChildren();
        for (XMLNode& xmlChild : xmlChildren)
        {
            bool modified = RefactorXMLInfo(&xmlChild, relPathBefore,
                                            relPathNow, true);
            hasBeenModified = hasBeenModified || modified;
        }
    }

    return hasBeenModified;
}

