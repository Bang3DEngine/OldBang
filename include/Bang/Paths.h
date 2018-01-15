#ifndef PATHS_H
#define PATHS_H

#include "Bang/Path.h"
#include "Bang/BinType.h"

NAMESPACE_BANG_BEGIN

#define EPATH(path)  Paths::CreateEnginePath(path)  // Engine assets path

class Paths
{
public:
    void InitPaths(const Path &engineRootPath);

    static Path GetHome();
    static Path GetExecutablePath();
    static const Path& GetEngineDir();
    static Path GetEngineAssetsDir();
    static Path GetEngineResourcesDir();
    static Path GetEngineLibrariesDir(BinType binaryType);
    static bool IsEnginePath(const Path &path);

    static Path CreateEnginePath(const String &path);
    static List<Path> GetEngineIncludeDirs();

    static void SetEngineRoot(const Path &engineRootDir);

    static void SortPathsByName(List<Path> *paths);
    static void SortPathsByExtension(List<Path> *paths);
    static void FilterByExtension(List<Path> *paths,
                                  const Array<String>& extensions);
    static void RemoveFilesFromList(List<Path> *paths);
    static void RemoveDirectoriesFromList(List<Path> *paths);

protected:
    Paths();
    virtual ~Paths();

    static Paths* GetInstance();

private:
    Path c_engineRoot  = Path::Empty;

    friend class Application;
};

NAMESPACE_BANG_END

#endif // PATHS_H
