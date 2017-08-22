#ifndef PATHS_H
#define PATHS_H

#include "Bang/Path.h"
#include "Bang/BinType.h"

#define EPATH(path) Paths::MakeEnginePath(path)  // Engine assets path
#define PPATH(path) Paths::MakeProjectPath(path) // Project assets path

class Paths
{
public:
    static const Path& Engine();
    static Path EngineAssets();
    static Path EngineBinaryDir(BinType binaryType);
    static Path EngineLibrariesDir(BinType binaryType);
    static Path GameExecutableOutputFile(BinType binaryType);
    static Path GameExecutableLibrariesDir(BinType binaryType);
    static bool IsEnginePath(const Path &path);

    static const Path& Project();
    static Path ProjectAssets();
    static Path ProjectLibrariesDir();

    static List<Path> GetAllProjectSubDirs();
    static List<Path> GetEngineIncludeDirs();
    static List<Path> GetProjectIncludeDirs();
    static List<Path> GetBehavioursSourcesFilepaths();

    static Path GetRelative(const Path &path);

    static Path MakeEnginePath(const String &path);
    static Path MakeProjectPath(const String &path);

    static void SetEngineRoot(const Path &engineRootDir);
    static void SetProjectRoot(const Path &projectRootDir);

private:
    Path c_engineRoot  = Path::Empty;
    Path c_projectRoot = Path::Empty;

    Paths();

    static Paths* GetInstance();

    friend class Application;
};

#endif // PATHS_H
