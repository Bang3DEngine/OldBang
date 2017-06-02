#include "Paths.h"

#include <QDir>
#include "Bang/WinUndef.h"

#include "Bang/String.h"
#include "Bang/SingletonManager.h"

Paths::Paths()
{

}

Path Paths::Home()
{
    return Path(QDir::homePath());
}

const Path &Paths::Engine()
{
    return Paths::GetInstance()->c_enginePath;
}

const Path &Paths::EngineAssets()
{
    return Paths::GetInstance()->c_engineAssetsPath;
}

const Path &Paths::Project()
{
    return Paths::GetInstance()->c_projectPath;
}

const Path &Paths::ProjectAssets()
{
    return Paths::GetInstance()->c_projectAssetsPath;
}

const Path &Paths::ProjectLibraries()
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
    return path.BeginsWith( Paths::Engine() );
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


void Paths::SetEnginePath(const Path &enginePath)
{
    Paths *p = Paths::GetInstance();
    p->c_enginePath       = enginePath;
    p->c_engineAssetsPath = p->c_enginePath.Append("EngineAssets");
}

void Paths::SetProjectPath(const Path &projectPath)
{
    Paths *p = Paths::GetInstance();
    p->c_projectPath          = projectPath;
    p->c_projectAssetsPath    = p->c_projectPath.Append("Assets");
    p->c_projectLibrariesPath = p->c_projectPath.Append("Libraries");
}
