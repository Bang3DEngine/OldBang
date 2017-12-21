#ifndef PATHS_H
#define PATHS_H

#include "Bang/Path.h"
#include "Bang/BinType.h"

NAMESPACE_BANG_BEGIN

#define EPATH(path)  Paths::MakeEnginePath(path)  // Engine assets path

class Paths
{
public:
    void InitPaths(const Path &engineRootPath);

    static Path ExecutablePath();
    static const Path& Engine();
    static Path EngineAssets();
    static Path EngineBinaryDir(BinType binaryType);
    static Path EngineLibrariesDir(BinType binaryType);
    static Path GameExecutableOutputFile(BinType binaryType);
    static Path GameExecutableLibrariesDir(BinType binaryType);
    static bool IsEnginePath(const Path &path);

    static List<Path> GetEngineIncludeDirs();
    static List<Path> GetBehavioursSourcesFilepaths();

    static Path GetRelative(const Path &path);

    static Path MakeEnginePath(const String &path);

    static void SetEngineRoot(const Path &engineRootDir);

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
