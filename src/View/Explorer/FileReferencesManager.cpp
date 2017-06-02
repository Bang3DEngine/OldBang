#include "Bang/FileReferencesManager.h"

#include <QTimer>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Paths.h"
#include "Bang/BFile.h"
#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/Explorer.h"
#include "Bang/FileTracker.h"
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
    QMutexLocker m(&m_mutex_inMemorySerialObjects);
    m_inMemorySerialObjects.Insert(fileable);
}

void FileReferencesManager::UnRegisterSerializableObject(SerializableObject *fileable)
{
    QMutexLocker m(&m_mutex_inMemorySerialObjects);
    m_inMemorySerialObjects.Remove(fileable);
}

void FileReferencesManager::OnFileOrDirNameMoved(const Path &filepathBefore,
                                                 const Path &filepathNow)
{
    ENSURE(!filepathBefore.IsEmpty());

    bool fileHasMoved = !filepathBefore.IsEmpty() &&
                        !filepathBefore.Exists() &&
                         filepathNow.Exists();
    if (fileHasMoved)
    {
        RefactorFiles(filepathBefore, filepathNow);
        RefactorSerializableObject(filepathBefore, filepathNow);
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
        frm->OnFileOrDirNameMoved(oldPath, newPath);
    }
}

FileReferencesManager *FileReferencesManager::GetInstance()
{
    Explorer *exp = Explorer::GetInstance();
    return exp ? exp->m_fileRefsManager : nullptr;
}

void FileReferencesManager::RefactorFiles(const Path &pathBefore,
                                          const Path &pathNow)
{
    List<Path> allFiles = Paths::ProjectAssets().GetFiles(true);
    for (const Path &filepath : allFiles)
    {
        BFile f(filepath);
        if (!f.IsAsset()) { continue; }

        String fileXMLContents = f.GetContents();
        XMLNode auxXMLInfo = XMLNode::FromString(fileXMLContents);
        if (RefactorXMLInfo(&auxXMLInfo, pathBefore, pathNow, true))
        {
            File::Write(filepath, auxXMLInfo.ToString());
        }
    }
}

void FileReferencesManager::RefactorSerializableObject(const Path &pathBefore,
                                                       const Path &pathNow)
{
    for (SerializableObject *serialObject : m_inMemorySerialObjects)
    {
        // First get its xml description, create an XMLNode, pass it to the
        // refactoring function, let the fileable read it to be updated, and
        // delete the created aux XMLNode
        String xmlInfoStr = serialObject->GetSerializedString();
        XMLNode auxXMLInfo = XMLNode::FromString(xmlInfoStr);
        if (RefactorXMLInfo(&auxXMLInfo, pathBefore, pathNow, false))
        {
            serialObject->Read(auxXMLInfo);
        }
    }
}

bool FileReferencesManager::RefactorXMLInfo(XMLNode *xmlInfo,
                                            const Path &pathBefore,
                                            const Path &pathNow,
                                            bool refactorXMLChildren)
{
    String relPathBefore = Paths::GetRelative(pathBefore).ToString();
    String relPathNow    = Paths::GetRelative(pathNow).ToString();
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
            bool modified = RefactorXMLInfo(&xmlChild, pathBefore,
                                            pathNow, true);
            hasBeenModified = hasBeenModified || modified;
        }
    }

    return hasBeenModified;
}

