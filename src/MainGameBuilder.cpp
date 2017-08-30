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
     "    (where DebugOrRelease can be 'Debug' or 'Release')"  << std::endl <<
     std::endl;
    exit(1);
}

int main(int argc, char **argv)
{
    Application app(argc, argv);

    if (argc <= 2) { Usage(); }

    String debugOrRelease(argv[2]);
    if (debugOrRelease != "Debug" && debugOrRelease != "Release") { Usage(); }
    BinType binaryType = (debugOrRelease == "Debug") ? BinType::Debug :
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
    String exeName = projectPath.GetName() + ".exe";
    GameBuilder::BuildGame(project,
                           projectPath.GetDirectory().Append(exeName),
                           binaryType,
                           compileBehaviours);

    return 0;
}
