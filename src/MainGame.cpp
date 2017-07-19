#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Project.h"
#include "Bang/Application.h"
#include "Bang/GameBuilder.h"
#include "Bang/ProjectManager.h"

int main(int argc, char **argv)
{
    Path gameProjectFilepath("GameData/Game.bproject");
    if (!gameProjectFilepath.Exists())
    {
        Debug_Error("Could not find " << gameProjectFilepath);
        return 1;
    }

    Paths::SetEngineRoot( gameProjectFilepath.GetDirectory() );
    Application app(argc, argv);

    Path projectPath(gameProjectFilepath);
    ProjectManager pm;
    Project *project = pm.OpenProject(projectPath);
    project->OpenFirstFoundScene();

    app.MainLoop();

    return 0;
}

