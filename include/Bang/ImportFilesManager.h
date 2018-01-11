#ifndef IMPORTFILESMANAGER_H
#define IMPORTFILESMANAGER_H

#include "Bang/Map.h"
#include "Bang/GUID.h"
#include "Bang/Path.h"
#include "Bang/GUIDManager.h"

NAMESPACE_BANG_BEGIN

class ImportFilesManager
{
public:
    static void CreateMissingImportFiles(const Path &directory);
    static void LoadImportFilepathGUIDs(const Path &directory);

    static Path CreateImportFile(const Path &filepath);
    static bool HasImportFile(const Path &filepath);
    static bool IsImportFile(const Path &filepath);
    static GUIDManager *GetGUIDManager();
    static void RegisterImportFilepath(const Path &importFilepath);
    static void UnRegisterImportFilepath(const Path &importFilepath);

    static Path GetFilepath(const GUID& guid);
    static Path GetFilepath(const Path& importFilepath);
    static Path GetImportFilepath(const Path &filepath);
    static const Path& GetImportFilepath(const GUID& guid);
    static void OnFilepathRenamed(const Path &oldPath, const Path &newPath);

    static GUID GetGUIDFromFilepath(const Path& filepath);
    static GUID GetGUIDFromImportFilepath(const Path& importFilepath);

private:
    Map<GUID, Path> m_GUIDToImportFilepath;
    Map<Path, GUID> m_importFilepathToGUID;

    GUIDManager m_GUIDManager;

    ImportFilesManager();
    ~ImportFilesManager();

    static String GetImportExtension();
    static ImportFilesManager* GetInstance();

    friend class Application;
};

NAMESPACE_BANG_END

#endif // IMPORTFILESMANAGER_H
