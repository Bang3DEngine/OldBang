#include "GameBuilder.h"

bool GameBuilder::BuildGame(const std::string &absoluteDir)
{
    bool ok = false;
    std::string output = "";
    SystemUtils::System("qmake \"BUILD_MODE=GAME\" && make", output, ok);

    if(!ok)
    {
        Logger_Error(output);
    }

    return ok;

    /*
    std::string includes = "";
    includes += SystemUtils::GetAllProjectSubDirs();
    includes += " . ";
    includes += SystemUtils::GetQtIncludes();
    StringUtils::RemoveLineBreaks(&includes);
    StringUtils::AddInFrontOfWords("-I", &includes);

    std::string objs = SystemUtils::GetAllProjectObjects();
    StringUtils::RemoveLineBreaks(&objs);

    std::string qtLibDirs = SystemUtils::GetQtLibrariesDirs();
    StringUtils::RemoveLineBreaks(&qtLibDirs);
    StringUtils::AddInFrontOfWords("-L", &qtLibDirs);

    // Gather options
    std::string options = "";
    options += " " + objs  + " ";
    options += " -O2";
    options += " -g ";
    // options += " -Wl,--export-dynamic ";
    options += " --std=c++11";
    options += " " + includes + " ";
    options += " -lGLEW -lGL -lpthread ";
    options += " " + qtLibDirs + " ";
    // options += " -fPIC"; // Shared linking stuff
    //

    std::string gameFilepath = absoluteDir + "/" + "game.exe";

    std::string cmd = "";
    cmd += "/usr/bin/g++ -shared ";
    cmd += filepath + " " + options + " -o " + gameFilepath;
    StringUtils::RemoveLineBreaks(&cmd);

    std::string output = "";
    bool ok = false;
    SystemUtils::System(cmd, output, ok);

    if (ok)
    {
        if(output != "")
        {
            Logger_Warn(output);
        }
    }
    else // There has been an error
    {
        if(output != "")
        {
            Logger_Error(output);
        }
        else
        {
            Logger_Error("There was an error compiling the Behaviour...");
        }
    }
    return ok;
*/
    return true;
}
