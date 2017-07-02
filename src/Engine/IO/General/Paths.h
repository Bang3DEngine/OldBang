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

    static Paths* GetInstance();

    static Path EnginePath(const String &path);
    static Path UserPath(const String &path);

private:
    Path c_enginePath;
    Path c_engineAssetsPath;
    Path c_engineBinaryDirPath;
    Path c_engineLibrariesDirPath;

    Path c_projectPath;
    Path c_projectAssetsPath;
    Path c_projectLibrariesPath;

    static void SetEngineBinaryDir(const Path &engineBinaryDir);
    static void SetEnginePath(const Path &enginePath);
    static void SetProjectPath(const Path &projectPath);

    friend class ProjectManager;
    friend int main(int argc, char** argv);
};

#endif // PATHS_H
