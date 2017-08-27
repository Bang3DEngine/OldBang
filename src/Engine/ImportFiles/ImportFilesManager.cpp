#include "Bang/ImportFilesManager.h"

#include "Bang/Set.h"
#include "Bang/File.h"
#include "Bang/List.h"
#include "Bang/Paths.h"
#include "Bang/XMLParser.h"
#include "Bang/Application.h"

ImportFilesManager::ImportFilesManager()
{
}

ImportFilesManager::~ImportFilesManager()
{
}

void ImportFilesManager::CreateMissingProjectImportFiles()
{
    // First load existing importFiles, to avoid creating new import files
    // with duplicated GUIDs.
    ImportFilesManager::LoadImportFilepathGUIDs();

    List<Path> projectFilesList = Paths::ProjectAssets().FindFiles(true);
    projectFilesList.PushBack( Paths::EngineAssets().FindFiles(true) );

    Set<Path> files;
    files.Add(projectFilesList.Begin(), projectFilesList.End());

    for (const Path &filepath : files)
    {
        if (!IsImportFile(filepath) && !HasImportFile(filepath))
        {
            ImportFilesManager::CreateImportFile(filepath);
        }
    }
}

void ImportFilesManager::LoadImportFilepathGUIDs()
{
    List<Path> importFilepaths =
            Paths::ProjectAssets().FindFiles(true, {GetImportExtension()});
    importFilepaths.PushBack( Paths::EngineAssets()
                              .FindFiles(true, {GetImportExtension()}) );

    ImportFilesManager *ifm = ImportFilesManager::GetInstance();
    for (const Path &importFilepath : importFilepaths)
    {
        XMLNode info = XMLParser::FromFile(importFilepath);

        GUID guid = info.Get<GUID>("GUID");
        Debug_Log(importFilepath << ": " << guid);
        ifm->m_GUIDToImportFilepath.Add(guid, importFilepath);
        ifm->m_importFilepathToGUID.Add(importFilepath, guid);
    }
}

Path ImportFilesManager::CreateImportFile(const Path &filepath)
{
    Path importFilepath;
    if ( !IsImportFile(filepath) && !HasImportFile(filepath) )
    {
        XMLNode xmlInfo;
        xmlInfo.Set( "GUID", GUIDManager::GetNewGUID() );
        importFilepath = GetImportFilePath(filepath);
        File::Write(importFilepath, xmlInfo.ToString());
    }
    return importFilepath;
}

bool ImportFilesManager::HasImportFile(const Path &filepath)
{
    return GetImportFilePath(filepath).Exists();
}

bool ImportFilesManager::IsImportFile(const Path &filepath)
{
    return filepath.HasExtension( GetImportExtension() );
}

GUIDManager* ImportFilesManager::GetGUIDManager()
{
    return &(ImportFilesManager::GetInstance()->m_GUIDManager);
}

GUID ImportFilesManager::GetGUIDFromFilepath(const Path& filepath)
{
    Path importFilepath = GetImportFilePath(filepath);
    Debug_Log("GUID for " << filepath << ": " << GetGUIDFromImportFilepath(importFilepath));
    return GetGUIDFromImportFilepath(importFilepath);
}

GUID ImportFilesManager::GetGUIDFromImportFilepath(const Path& importFilepath)
{
    XMLNode xmlNode = XMLParser::FromFile(importFilepath);
    Debug_Log("GUID for " << importFilepath << ": " << xmlNode.Get<GUID>("GUID"));
    return xmlNode.Get<GUID>("GUID");
}

Path ImportFilesManager::GetFilepath(const GUID &guid)
{
    Path importFilepath = ImportFilesManager::GetImportFilepath(guid);
    return ImportFilesManager::GetFilepath(importFilepath);
}

Path ImportFilesManager::GetFilepath(const Path &importFilepath)
{
    String strPath = importFilepath.GetAbsolute();
    if (strPath.BeginsWith(".")) { strPath.Remove(0, 0); }

    String ending = "." + GetImportExtension();
    if (strPath.EndsWith(ending))
    {
        strPath.Remove(strPath.Size() - ending.Size(),
                       strPath.Size()-1);
    }

    return Path(strPath);
}

Path ImportFilesManager::GetImportFilePath(const Path &filepath)
{
    return filepath.AppendExtension( GetImportExtension() ).ChangeHidden(true);
}

const Path &ImportFilesManager::GetImportFilepath(const GUID &guid)
{
    ImportFilesManager *ifm = ImportFilesManager::GetInstance();
    if (ifm->m_GUIDToImportFilepath.ContainsKey(guid))
    {
        return ifm->m_GUIDToImportFilepath.Get(guid);
    }
    return Path::Empty;
}

ImportFilesManager *ImportFilesManager::GetInstance()
{
    return Application::GetInstance()->GetImportFilesManager();
}

String ImportFilesManager::GetImportExtension()
{
    return "import";
}
