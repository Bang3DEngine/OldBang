#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Application.h"
#include "Bang/GameBuilder.h"
#include "Bang/ProjectManager.h"

void Usage()
{
    std::cerr <<
     "Usage:" << std::endl <<
     "   ./BangGameBuilder Path/To/Project.bproject DebugOrRelease" << std::endl <<
     "    (where DebugOrRelease can be 'DEBUG' or 'RELEASE')"  << std::endl <<
     std::endl;
    exit(1);
}

int main(int argc, char **argv)
{
    Application app(argc, argv);

    if (argc <= 2) { Usage(); }

    String debugOrRelease(argv[2]);
    if (debugOrRelease != "DEBUG" && debugOrRelease != "RELEASE") { Usage(); }
    BinType binaryType = (debugOrRelease == "DEBUG") ? BinType::Debug :
                                                       BinType::Release;

    bool compileBehaviours = true;
    for (char **arg = argv; arg < argv + argc; ++arg)
    {
        if (strcmp(*arg, "NO_COMPILE_BEHAVIOURS") == 0)
        {
            compileBehaviours = false;
        }
    }

    Path projectPath(argv[1]);
    ProjectManager pm;
    Project *project = pm.OpenProject(projectPath);
    GameBuilder::BuildGame(project,
                           projectPath.GetDirectory().Append("Snake.exe"),
                           binaryType,
                           compileBehaviours);

    return 0;
}
