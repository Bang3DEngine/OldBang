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

    static const Path& Engine();
    static const Path& EngineAssets();

    static const Path& Project();
    static const Path& ProjectAssets();
    static const Path& ProjectLibraries();

    static Path GetRelative(const Path &path);

    static bool IsEnginePath(const Path &path);

    static Paths* GetInstance();

    static Path EnginePath(const String &path);
    static Path UserPath(const String &path);

private:
    Path c_enginePath;
    Path c_engineAssetsPath;

    Path c_projectPath;
    Path c_projectAssetsPath;
    Path c_projectLibrariesPath;

    static void SetEnginePath(const Path &enginePath);
    static void SetProjectPath(const Path &projectPath);

    friend class ProjectManager;
    friend int main(int argc, char** argv);
};

#endif // PATHS_H
