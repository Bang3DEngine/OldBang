#include "Bang/ImportFilesManager.h"

#include "Bang/Set.h"
#include "Bang/File.h"
#include "Bang/List.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Application.h"
#include "Bang/XMLNodeReader.h"

USING_NAMESPACE_BANG

ImportFilesManager::ImportFilesManager()
{
}

ImportFilesManager::~ImportFilesManager()
{
}

void ImportFilesManager::CreateMissingImportFiles(const Path &directory)
{
    // First load existing importFiles, to avoid creating new import files
    // with duplicated GUIDs.
    ImportFilesManager::LoadImportFilepathGUIDs(directory);

    List<Path> assetFile = directory.GetFiles(Path::FindFlag::Recursive);

    Set<Path> files;
    files.Add(assetFile.Begin(), assetFile.End());

    for (const Path &filepath : files)
    {
        if (!IsImportFile(filepath) && !HasImportFile(filepath))
        {
            ImportFilesManager::CreateImportFile(filepath);
        }
    }
}

void ImportFilesManager::LoadImportFilepathGUIDs(const Path &directory)
{
    Array<String> extensions = {GetImportExtension()};
    List<Path> importFilepaths = directory.GetFiles(Path::FindFlag::RecursiveHidden,
                                                    extensions);

    // Remove alone .import files
    for (const Path &importFilepath : importFilepaths)
    {
        if ( IsImportFile(importFilepath) &&
            !GetFilepath(importFilepath).IsFile() )
        {
            File::Remove(importFilepath);
        }
    }

    // Load GUID's of import files!
    for (const Path &importFilepath : importFilepaths)
    {
        RegisterImportFilepath(importFilepath);
    }
}

std::pair<Path, GUID> ImportFilesManager::CreateImportFile(const Path &filepath)
{
    Path importFilepath;
    GUID newGUID = GUID::Empty();
    if ( !IsImportFile(filepath) && !HasImportFile(filepath) )
    {
        XMLNode xmlInfo;
        newGUID = GUIDManager::GetNewGUID();
        xmlInfo.Set("GUID", newGUID);
        importFilepath = GetImportFilepath(filepath);
        File::Write(importFilepath, xmlInfo.ToString());
    }
    return std::make_pair(importFilepath, newGUID);
}

bool ImportFilesManager::HasImportFile(const Path &filepath)
{
    return GetImportFilepath(filepath).Exists();
}

bool ImportFilesManager::IsImportFile(const Path &filepath)
{
    return filepath.IsHiddenFile() &&
           filepath.HasExtension( GetImportExtension() );
}

void ImportFilesManager::DuplicateImportFile(const Path &filepath,
                                             const Path &dupFilepath)
{
    const Path dupImportFilepath = GetImportFilepath(dupFilepath);
    File::Remove(dupImportFilepath);

    const GUID& newGUID = CreateImportFile(dupFilepath).second;
    const Path originalImportFilepath = GetImportFilepath(filepath);
    XMLNode originalXML = XMLNodeReader::FromFile(originalImportFilepath);
    originalXML.Set("GUID", newGUID);

    File::Write(dupImportFilepath, originalXML.ToString());
    RegisterImportFilepath(dupImportFilepath);
}

GUIDManager* ImportFilesManager::GetGUIDManager()
{
    return &(ImportFilesManager::GetInstance()->m_GUIDManager);
}

void ImportFilesManager::RegisterImportFilepath(const Path &importFilepath)
{
    if (!IsImportFile(importFilepath)) { return; }

    XMLNode info = XMLNodeReader::FromFile(importFilepath);

    GUID guid = info.Get<GUID>("GUID");
    ImportFilesManager *ifm = ImportFilesManager::GetInstance();
    if (ifm->m_GUIDToImportFilepath.ContainsKey(guid) &&
        ifm->m_GUIDToImportFilepath.Get(guid) != importFilepath)
    {
        Debug_Error("Found conflicting GUID: " << guid <<
                    "(Files '" << importFilepath << "' and '" <<
                    ifm->m_GUIDToImportFilepath.Get(guid) << "'");
    }

    ifm->m_GUIDToImportFilepath.Add(guid, importFilepath);
    ifm->m_importFilepathToGUID.Add(importFilepath, guid);
}

void ImportFilesManager::UnRegisterImportFilepath(const Path &importFilepath)
{
    ImportFilesManager *ifm = ImportFilesManager::GetInstance();
    ifm->m_GUIDToImportFilepath.RemoveValues(importFilepath);
    ifm->m_importFilepathToGUID.Remove(importFilepath);
}

GUID ImportFilesManager::GetGUIDFromFilepath(const Path& filepath)
{
    Path importFilepath = GetImportFilepath(filepath);
    return GetGUIDFromImportFilepath(importFilepath);
}

GUID ImportFilesManager::GetGUIDFromImportFilepath(const Path& importFilepath)
{
    XMLNode xmlNode = XMLNodeReader::FromFile(importFilepath);
    return xmlNode.Get<GUID>("GUID");
}

Path ImportFilesManager::GetFilepath(const GUID &guid)
{
    Path importFilepath = ImportFilesManager::GetImportFilepath(guid);
    return ImportFilesManager::GetFilepath(importFilepath);
}

Path ImportFilesManager::GetFilepath(const Path &importFilepath)
{
    Path filepath = importFilepath.WithHidden(false);

    String strPath = filepath.GetAbsolute();
    if (strPath.BeginsWith(".")) { strPath.Remove(0, 1); }

    String ending = "." + GetImportExtension();
    if (strPath.EndsWith(ending))
    {
        strPath.Remove(strPath.Size() - ending.Size(), strPath.Size());
    }

    return Path(strPath);
}

Path ImportFilesManager::GetImportFilepath(const Path &filepath)
{
    return filepath.AppendExtension( GetImportExtension() ).WithHidden(true);
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

void ImportFilesManager::OnFilepathRenamed(const Path &oldPath, const Path &newPath)
{
    Path oldImportFilepath = ImportFilesManager::GetImportFilepath(oldPath);
    ImportFilesManager::UnRegisterImportFilepath(oldImportFilepath);

    Path newImportFilepath = ImportFilesManager::GetImportFilepath(newPath);

    File::Rename(oldImportFilepath, newImportFilepath);
    ImportFilesManager::RegisterImportFilepath(newImportFilepath);
}

ImportFilesManager *ImportFilesManager::GetInstance()
{
    return Application::GetInstance()->GetImportFilesManager();
}

String ImportFilesManager::GetImportExtension()
{
    return "import";
}
