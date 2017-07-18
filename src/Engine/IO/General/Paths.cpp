#include "Paths.h"

#include <QDir>

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/SystemUtils.h"

Path Paths::c_enginePath = Path::Empty;
Path Paths::c_engineAssetsPath = Path::Empty;
Path Paths::c_engineBinaryDirPath = Path::Empty;
Path Paths::c_engineLibrariesDirPath = Path::Empty;

Path Paths::c_projectPath = Path::Empty;
Path Paths::c_projectAssetsPath = Path::Empty;
Path Paths::c_projectLibrariesPath = Path::Empty;

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
    return Paths::c_engineBinaryDirPath;
}

const Path &Paths::EngineLibrariesDir()
{
    return Paths::c_engineLibrariesDirPath;
}

const Path &Paths::Engine()
{
    return Paths::c_enginePath;
}

const Path &Paths::EngineAssets()
{
    return Paths::c_engineAssetsPath;
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
    return Paths::c_projectPath;
}

const Path &Paths::ProjectAssets()
{
    return Paths::c_projectAssetsPath;
}

const Path &Paths::ProjectLibrariesDir()
{
    return Paths::c_projectLibrariesPath;
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
    Paths::c_engineBinaryDirPath = engineBinaryDir;
    Paths::c_engineLibrariesDirPath = Paths::c_engineBinaryDirPath.Append("lib");
}


void Paths::SetEnginePath(const Path &enginePath)
{
    Paths::c_enginePath       = enginePath;
    Paths::c_engineAssetsPath = Paths::c_enginePath.Append("res")
                                                   .Append("EngineAssets");
}

void Paths::SetProjectPath(const Path &projectPath)
{
    Paths::c_projectPath          = projectPath;
    Paths::c_projectAssetsPath    = Paths::c_projectPath.Append("Assets");
    Paths::c_projectLibrariesPath = Paths::c_projectPath.Append("Libraries");
}
