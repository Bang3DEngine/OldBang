#include "Bang/Paths.h"

#include <limits.h>
#include <unistd.h>

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Application.h"
#include "Bang/SystemUtils.h"

USING_NAMESPACE_BANG

Paths::Paths()
{
}

Paths::~Paths()
{

}

void Paths::InitPaths(const Path &engineRootPath)
{
    c_engineRoot = Path::Empty;
    if (!engineRootPath.IsEmpty())
    {
        c_engineRoot = engineRootPath;
    }

    // Try current directory (some dirs up)
    if (!EngineAssets().IsDir())
    {
        c_engineRoot = Paths::ExecutablePath().GetDirectory()
                                              .GetDirectory()
                                              .GetDirectory();
    }

    // Try default installation path
    if (!EngineAssets().IsDir())
    {
        c_engineRoot = Path("/opt/Bang");
        if (!EngineAssets().IsDir())
        {
            Debug_Error("Bang is not properly installed. "
                        "Can't find it in /opt.");
            Application::Exit(1, true);
        }
    }

    Debug_Log("Picking as Paths Bang Engine Root: " << Engine());
}

Path Paths::ExecutablePath()
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX );
    String exePath( std::string(result, (count > 0) ? count : 0) );
    return Path(exePath);
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
    return List<Path>(); // Paths::ProjectAssets().FindFiles(Path::FindFlag::Recursive, {"cpp"});
}

List<Path> Paths::GetEngineIncludeDirs()
{
    Path incPath = Paths::Engine().Append("include");
    List<Path> subdirs;
    subdirs.PushBack(incPath.FindSubDirectories(Path::FindFlag::Simple));
    subdirs.PushBack(incPath.Append("Bang")
                     .FindSubDirectories(Path::FindFlag::Simple));
    return subdirs;
}

Path Paths::GetRelative(const Path &path)
{
    const Path &engineAssets = Paths::EngineAssets();
    if (path.BeginsWith(engineAssets))
    {
        return Path(path.GetAbsolute()
                        .SubString(engineAssets.GetAbsolute().Size() + 1));
    }

    const Path &engineRoot = Paths::Engine();
    if (path.BeginsWith(engineRoot))
    {
        return Path(path.GetAbsolute()
                        .SubString(engineRoot.GetAbsolute().Size() + 1));
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

void Paths::SetEngineRoot(const Path &engineRootDir)
{
    Paths::GetInstance()->c_engineRoot = engineRootDir;
}

Paths *Paths::GetInstance()
{
    return Application::GetInstance()->GetPaths();
}
