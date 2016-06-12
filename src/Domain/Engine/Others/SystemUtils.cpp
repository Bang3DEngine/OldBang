#include "SystemUtils.h"

#include "Behaviour.h"
#include "WindowMain.h"

// TODO: Fix System success variable (it does not do what it's supposed to do)
void SystemUtils::System(const std::string &command, std::string &output, bool &success)
{
    const int buff_size = 1024 * 512;
    char buff[buff_size + 1];
    memset((char*)&buff, 0, buff_size + 1);

    const char *cmd = command.c_str();
    char *ret = nullptr;
    std::string str = "";

    int fd[2];
    int old_fd[3];
    int r = pipe(fd);
    success = true;

    old_fd[0] = !STDIN_FILENO;
    old_fd[1] = !STDOUT_FILENO;
    old_fd[2] = !STDERR_FILENO;

    old_fd[0] = dup(STDIN_FILENO);
    old_fd[1] = dup(STDOUT_FILENO);
    old_fd[2] = dup(STDERR_FILENO);

    int pid = fork();
    if(pid == 0) // Child
    {
        close(fd[0]);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        dup2(fd[1], STDERR_FILENO);

        int r = system(cmd);

        close (fd[1]);
        exit(1);
    }
    else if(pid == -1)
    {
        success = false;
        exit(1);
    }

    if(pid != 0) // Parent
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        while (read(fd[0], buff, buff_size))
        {
            str.append(buff);
            memset(buff, 0, buff_size);
        }

        ret = (char*) malloc( str.size() );
        strcpy(ret, (char*) str.c_str());

        waitpid(pid, nullptr, 0);
        close(fd[0]);
    }

    dup2(STDIN_FILENO,  old_fd[0]);
    dup2(STDOUT_FILENO, old_fd[1]);
    dup2(STDERR_FILENO, old_fd[2]);

    if(ret != nullptr && strlen(ret) != 0)
    {
        output = std::string(ret);
    }
}


std::string SystemUtils::CompileToSharedObject(const std::string &filepathFromProjectRoot)
{

    // GET INCLUDES
    // Get all subdirs recursively in a single line, and add -I in front of every path
    std::string cmdGetAllSubDirs = "";
    cmdGetAllSubDirs = "find " +                                    // Find recursively
                        Persistence::GetProjectRootPathAbsolute() + // From project root
                        " -type d " +                               // Only directories
                        " | grep -E -v \"\\.\" " +                  // Not including hidden dirs
                        " | xargs";                                 // Inline

    bool ok = false;
    std::string allSubDirs = "";
    SystemUtils::System(cmdGetAllSubDirs, allSubDirs, ok);

    if(!ok)
    {
        Logger_Error("Error trying to find include directories to compile " <<
                     filepathFromProjectRoot);
        return "";
    }

    // Add -I in front of every path
    std::string includes = "-I" + allSubDirs;
    for(int i = 0; i < includes.length(); ++i)
    {
        if(includes[i] == ' ')
        {
            includes.insert(i+1, "-I");
            i += 2; // Sorry
        }
    }
    //

    includes += " -I.";

    #ifdef BANG_EDITOR
    // Qt includes
    includes += " -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui";
    includes += " -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4";
    //
    #endif
    //

    // GET OBJS (*.o)
    std::string cmdGetAllObjects = "";
    cmdGetAllObjects = "find " +                                    // Find recursively
                        Persistence::GetProjectRootPathAbsolute() + // From project root
                        " -type f " +                               // Only files
                        " | grep -E -v \"\\..*/.*\" " +             // Not including hidden dirs
                        " | grep -E -v \"Preprocessor\" " +         // Temporal fix with colliding .o's TODO
                        " | grep -E -v \"main\\.o\" " +             // Temporal fix with colliding .o's TODO
                        " | grep -E \"\\.o$\"" +                    // Only .o files
                        " | xargs";                                 // Inline
    std::string objs = "";
    SystemUtils::System(cmdGetAllObjects, objs, ok);

    if(!ok)
    {
        Logger_Error("Error trying to find object files to compile " <<
                     filepathFromProjectRoot);
        return "";
    }
    //

    // Gather options
    std::string options = "";
    options += " " + objs;
    options += " -O1";
    options += " -g ";
    options += " -Wl,--export-dynamic ";
    options += " --std=c++11";
    options += " " + includes;
    options += " -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib64 ";
    options += " -lGLEW -lQtOpenGL -lQtGui -lQtCore -lGL -lpthread ";
    options += " -fPIC"; // Shared linking stuff
    //

    std::string filepath =
            Persistence::ProjectRootRelativeToAbsolute(filepathFromProjectRoot);
    std::string scriptDir = Persistence::GetDir(filepath);
    std::string scriptName = Persistence::GetFileNameWithExtension(filepath);

    // Compile
    std::string sharedObjectFilepath = "";
    sharedObjectFilepath += scriptDir + "/" + scriptName + ".so";

    std::string cmd = "";
    cmd += "/usr/bin/g++ -shared ";
    cmd += filepath + " " + options + " -o " + sharedObjectFilepath;

    std::string output = "";
    std::replace(cmd.begin(), cmd.end(), '\n', ' '); // Remove line breaks
    SystemUtils::System(cmd, output, ok);
    Logger_Log(cmd);
    if(output != "")
    {
        Logger_Log(output);
        if (!ok)
        {
            Logger_Error(output);
            return "";
        }
    }

    return sharedObjectFilepath;
}

Behaviour* SystemUtils::CreateDynamicBehaviour(const std::string &sharedObjectFilepath)
{
    dlerror(); // Clear last error

    // Open library
    void *lib = dlopen(sharedObjectFilepath.c_str(), RTLD_LAZY | RTLD_GLOBAL);

    // Error Check
    char *err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
        return nullptr;
    }

    if(lib == nullptr) return nullptr;

    // Get the pointer to the CreateDynamically function
    Behaviour* (*createFunction)(WindowMain*) =
            (Behaviour* (*)(WindowMain*)) (dlsym(lib, "CreateDynamically"));

    // Error Check
    err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
        return nullptr;
    }

    // Call it and get the pointer to the created Behaviour
    Behaviour *b = nullptr;
    if(createFunction != nullptr)
    {
        b = createFunction(WindowMain::GetInstance());
    }

    return b;
}

