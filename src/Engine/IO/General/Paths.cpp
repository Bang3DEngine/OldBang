#include "Paths.h"

#include <QDir>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/SystemUtils.h"
#include "Bang/SingletonManager.h"

Paths::Paths()
{

}

Path Paths::Home()
{
    return Path(QDir::homePath());
}

Path Paths::GameExecutableLibrariesDir()
{
    return Paths::EngineBinaryDir()
                    .GetDirectory().Append("GameRelease/lib");
}

Path Paths::GameExecutableOutputFile()
{
    return Paths::EngineBinaryDir()
                    .GetDirectory()
                    .Append("GameRelease/Game")
                    .AppendExtension("exe");
}

const Path &Paths::EngineBinaryDir()
{
    return Paths::GetInstance()->c_engineBinaryDirPath;
}

const Path &Paths::EngineLibrariesDir()
{
    return Paths::GetInstance()->c_engineLibrariesDirPath;
}

const Path &Paths::Engine()
{
    return Paths::GetInstance()->c_enginePath;
}

const Path &Paths::EngineAssets()
{
    return Paths::GetInstance()->c_engineAssetsPath;
}

List<Path> Paths::GetBehavioursSourcesFilepaths()
{
    return Paths::ProjectAssets().GetFiles(true, {"cpp"});
}

List<Path> Paths::GetAllProjectSubDirs()
{
    List<Path> subdirs = Paths::Project().GetSubDirectories(true);
    subdirs.PushFront(Paths::Project());
    return subdirs;
}

List<Path> Paths::GetAllEngineSubDirs()
{
    List<Path> subdirs = Paths::Engine().GetSubDirectories(true);
    subdirs.PushFront(Paths::Engine());
    return subdirs;
}

const List<Path> &Paths::GetQtIncludeDirs()
{
    static List<Path> qtIncludeDirs;
    if (qtIncludeDirs.IsEmpty())
    {
        bool ok = false;
        String qtDir = "";
        SystemUtils::System("qmake", {"-query", "QT_INSTALL_HEADERS"},
                            &qtDir, &ok);
        if (!ok)
        {
            Debug_Error("Error trying to find Qt include directories to compile.");
            return qtIncludeDirs;
        }
        qtDir = qtDir.Replace("\n", "");

        qtIncludeDirs = Path(qtDir).GetSubDirectories(true);
        for (auto it = qtIncludeDirs.Begin(); it != qtIncludeDirs.End();)
        {
            const Path& libPath = *it;
            const String& incDir = libPath.GetAbsolute();
            if (!incDir.Contains("qt", false) || !libPath.IsDir())
            {
                qtIncludeDirs.Remove(it++);
            }
            else { ++it; }
        }
        qtIncludeDirs.PushFront( Path(qtDir) );
    }
    return qtIncludeDirs;
}

const List<Path> &Paths::GetQtLibrariesDirs()
{
    static List<Path> qtLibDirs;
    if (qtLibDirs.IsEmpty())
    {
        bool ok = false;
        String qtDir = "";
        SystemUtils::System("qmake", {"-query", "QT_INSTALL_LIBS"},
                            &qtDir, &ok);
        if (!ok)
        {
            Debug_Error("Error trying to find Qt library directories to compile.");
            return qtLibDirs;
        }
        qtDir = qtDir.Replace("\n", "");

        qtLibDirs = Path(qtDir).GetSubDirectories(true);
        for (auto it = qtLibDirs.Begin(); it != qtLibDirs.End();)
        {
            const Path& libDirPath = *it;
            const String& libDir = libDirPath.GetAbsolute();
            if ( !libDir.Contains("qt", false) || !libDirPath.IsDir() )
            {
                it = qtLibDirs.Remove(it++);
            }
            else { ++it; }
        }
        qtLibDirs.PushFront( Path(qtDir) );
    }
    return qtLibDirs;
}

const Path &Paths::Project()
{
    return Paths::GetInstance()->c_projectPath;
}

const Path &Paths::ProjectAssets()
{
    return Paths::GetInstance()->c_projectAssetsPath;
}

const Path &Paths::ProjectLibrariesDir()
{
    return Paths::GetInstance()->c_projectLibrariesPath;
}

Path Paths::GetRelative(const Path &path)
{
    const Path &engineAssets = Paths::EngineAssets();
    if (path.BeginsWith(engineAssets))
    {
        return Path(path.GetAbsolute()
                        .SubString(engineAssets.GetAbsolute().Length() + 1));
    }

    const Path &projectAssets = Paths::ProjectAssets();
    if (path.BeginsWith(projectAssets))
    {
        return Path(path.GetAbsolute()
                        .SubString(projectAssets.GetAbsolute().Length() + 1));
    }

    const Path &engineRoot = Paths::Engine();
    if (path.BeginsWith(engineRoot))
    {
        return Path(path.GetAbsolute()
                        .SubString(engineRoot.GetAbsolute().Length() + 1));
    }

    const Path &projectRoot = Paths::Project();
    if (path.BeginsWith(projectRoot))
    {
        return Path(path.GetAbsolute()
                        .SubString(projectRoot.GetAbsolute().Length() + 1));
    }

    return Path(path.GetAbsolute());
}

bool Paths::IsEnginePath(const Path &path)
{
    return path.BeginsWith( Paths::Engine() + "/" );
}

Paths *Paths::GetInstance()
{
    Paths *paths = SingletonManager::Get<Paths>();
    if (!paths) { paths = SingletonManager::Set<Paths>(new Paths()); }
    return paths;
}

Path Paths::EnginePath(const String &path)
{
    return Paths::EngineAssets().Append(path);
}

Path Paths::UserPath(const String &path)
{
    return Paths::ProjectAssets().Append(path);
}

void Paths::SetEngineBinaryDir(const Path &engineBinaryDir)
{
    Paths *p = Paths::GetInstance();
    p->c_engineBinaryDirPath = engineBinaryDir;
    p->c_engineLibrariesDirPath = p->c_engineBinaryDirPath.Append("lib");
}


void Paths::SetEnginePath(const Path &enginePath)
{
    Paths *p = Paths::GetInstance();
    p->c_enginePath       = enginePath;
    p->c_engineAssetsPath = p->c_enginePath.Append("res")
                                           .Append("EngineAssets");
}

void Paths::SetProjectPath(const Path &projectPath)
{
    Paths *p = Paths::GetInstance();
    p->c_projectPath          = projectPath;
    p->c_projectAssetsPath    = p->c_projectPath.Append("Assets");
    p->c_projectLibrariesPath = p->c_projectPath.Append("Libraries");
}
