#include "Paths.h"

#include <limits.h>
#include <unistd.h>

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Application.h"
#include "Bang/SystemUtils.h"

Paths::Paths()
{
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX );
  String strPath( std::string(result, (count > 0) ? count : 0) );
  c_engineRoot = Path(strPath).GetDirectory()
                              .GetDirectory()
                              .GetDirectory();
}

const Path &Paths::Engine()
{
    return Paths::GetInstance()->c_engineRoot;
}
Path Paths::EngineAssets()
{
    return Engine().Append("res/EngineAssets");
}
Path Paths::EngineBinaryDir(BinType binaryType)
{
    return Engine()
              .Append("bin")
              .Append( binaryType == BinType::Debug ? "Debug" : "Release" );
}
Path Paths::EngineLibrariesDir(BinType binaryType)
{
    return Paths::EngineBinaryDir(binaryType).Append("lib");
}
Path Paths::GameExecutableOutputFile(BinType binaryType)
{
    return Paths::EngineBinaryDir(binaryType).Append("Game.exe");
}

List<Path> Paths::GetBehavioursSourcesFilepaths()
{
    return Paths::ProjectAssets().FindFiles(true, {"cpp"});
}

List<Path> Paths::GetAllProjectSubDirs()
{
    List<Path> subdirs = Paths::Project().FindSubDirectories(true);
    subdirs.PushFront(Paths::Project());
    return subdirs;
}

List<Path> Paths::GetProjectIncludeDirs()
{
    List<Path> subdirs = Paths::ProjectAssets().FindSubDirectories(true);
    return subdirs;
}

List<Path> Paths::GetEngineIncludeDirs()
{
    Path incPath = Paths::Engine().Append("include");
    List<Path> subdirs;
    subdirs.PushBack(incPath.FindSubDirectories(false));
    subdirs.PushBack(incPath.Append("Bang").FindSubDirectories(false));
    return subdirs;
}

const Path &Paths::Project()
{
    return Paths::GetInstance()->c_projectRoot;
}

Path Paths::ProjectAssets()
{
    return Project().Append("Assets");
}

Path Paths::ProjectLibrariesDir()
{
    return Project().Append("Libraries");
}

Path Paths::GetRelative(const Path &path)
{
    const Path &engineAssets = Paths::EngineAssets();
    if (path.BeginsWith(engineAssets))
    {
        return Path(path.GetAbsolute()
                        .SubString(engineAssets.GetAbsolute().Size() + 1));
    }

    const Path &projectAssets = Paths::ProjectAssets();
    if (path.BeginsWith(projectAssets))
    {
        return Path(path.GetAbsolute()
                        .SubString(projectAssets.GetAbsolute().Size() + 1));
    }

    const Path &engineRoot = Paths::Engine();
    if (path.BeginsWith(engineRoot))
    {
        return Path(path.GetAbsolute()
                        .SubString(engineRoot.GetAbsolute().Size() + 1));
    }

    const Path &projectRoot = Paths::Project();
    if (path.BeginsWith(projectRoot))
    {
        return Path(path.GetAbsolute()
                        .SubString(projectRoot.GetAbsolute().Size() + 1));
    }

    return Path(path.GetAbsolute());
}

bool Paths::IsEnginePath(const Path &path)
{
    return path.BeginsWith( Paths::Engine() + "/" );
}

Path Paths::MakeEnginePath(const String &path)
{
    return Paths::EngineAssets().Append(path);
}

Path Paths::MakeProjectPath(const String &path)
{
    return Paths::ProjectAssets().Append(path);
}

void Paths::SetEngineRoot(const Path &engineRootDir)
{
    Paths::GetInstance()->c_engineRoot = engineRootDir;
}

void Paths::SetProjectRoot(const Path &projectRootDir)
{
    Paths::GetInstance()->c_projectRoot = projectRootDir;
}

Paths *Paths::GetInstance()
{
    return Application::GetInstance()->GetPaths();
}
