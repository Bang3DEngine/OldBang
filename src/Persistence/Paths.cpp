#include "Paths.h"

#include <QDir>
#include "Bang/WinUndef.h"

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
