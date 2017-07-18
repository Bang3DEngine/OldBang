#ifndef PATHS_H
#define PATHS_H

#include "Bang/Path.h"

#define EPATH(path) Paths::EnginePath(path) // Engine assets path
#define UPATH(path) Paths::UserPath(path)   // User assets path

class Paths
{
public:
    Paths();

    static Path Home();

    static Path GameExecutableLibrariesDir();
    static Path GameExecutableOutputFile();
    static const Path& EngineBinaryDir();
    static const Path& EngineLibrariesDir();

    static const Path& Engine();
    static const Path& EngineAssets();

    static List<Path> GetBehavioursSourcesFilepaths();
    static List<Path> GetAllProjectSubDirs();
    static List<Path> GetAllEngineSubDirs();
    static const List<Path>& GetQtIncludeDirs();
    static const List<Path>& GetQtLibrariesDirs();

    static const Path& Project();
    static const Path& ProjectAssets();
    static const Path& ProjectLibrariesDir();

    static Path GetRelative(const Path &path);

    static bool IsEnginePath(const Path &path);

    static Path EnginePath(const String &path);
    static Path UserPath(const String &path);

    static void SetEngineBinaryDir(const Path &engineBinaryDir);
    static void SetEnginePath(const Path &enginePath);
    static void SetProjectPath(const Path &projectPath);

private:
    static Path c_enginePath;
    static Path c_engineAssetsPath;
    static Path c_engineBinaryDirPath;
    static Path c_engineLibrariesDirPath;

    static Path c_projectPath;
    static Path c_projectAssetsPath;
    static Path c_projectLibrariesPath;
};

#endif // PATHS_H
