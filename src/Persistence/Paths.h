#ifndef PATHS_H
#define PATHS_H

#include "Bang/Path.h"

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

    static bool IsEnginePath(const Path &path);

    static Paths* GetInstance();

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
