#ifndef IMPORTFILESMANAGER_H
#define IMPORTFILESMANAGER_H

#include "Bang/Map.h"
#include "Bang/GUID.h"
#include "Bang/Path.h"
#include "Bang/GUIDManager.h"

class ImportFilesManager
{
public:
    static void CreateMissingProjectImportFiles();
    static void LoadImportFilepathGUIDs();

    static Path CreateImportFile(const Path &filepath);
    static bool HasImportFile(const Path &filepath);
    static bool IsImportFile(const Path &filepath);
    static GUIDManager *GetGUIDManager();

    static Path GetFilepath(const GUID& guid);
    static Path GetFilepath(const Path& importFilepath);
    static Path GetImportFilePath(const Path &filepath);
    static const Path& GetImportFilepath(const GUID& guid);

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

#endif // IMPORTFILESMANAGER_H
