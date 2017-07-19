#ifndef PATHS_H
#define PATHS_H

#include "Bang/Path.h"
#include "Bang/BinType.h"

#define EPATH(path) Paths::MakeEnginePath(path)  // Engine assets path
#define PPATH(path) Paths::MakeProjectPath(path) // Project assets path

class Paths
{
public:
    Paths();

    static void InitPaths(int argc, char **argv);
    static void InitPaths(const Path &engineRoot);

    static Path Home();

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
    static List<Path> GetAllEngineSubDirs();
    static const List<Path>& GetQtIncludeDirs();
    static const List<Path>& GetQtLibrariesDirs();
    static List<Path> GetBehavioursSourcesFilepaths();

    static Path GetRelative(const Path &path);

    static Path MakeEnginePath(const String &path);
    static Path MakeProjectPath(const String &path);

    static void SetEngineRoot(const Path &engineRootDir);
    static void SetProjectRoot(const Path &projectRootDir);

private:
    static Path c_engineRoot;
    static Path c_projectRoot;
};

#endif // PATHS_H
