#include "SystemUtils.h"

#include "Behaviour.h"
#include "SingletonManager.h"

// TODO: Fix System success variable (it does not do what it's supposed to do)
void SystemUtils::System(const std::string &command, std::string &output, bool &success)
{
    int fd[2];
    int old_fd[3] = {dup(STDIN_FILENO), dup(STDOUT_FILENO), dup(STDERR_FILENO)};
    int pipeResult = pipe(fd);
    if (pipeResult != 0)
    {
        output = "There was an error when creating a pipe to compile the Behaviour.";
        success = false;
        return;
    }

    int pid = fork();
    if (pid == 0) // Child
    {
        close(fd[0]);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        dup2(fd[1], STDERR_FILENO);

        int result = system(command.c_str());

        close (fd[1]);

        // This is needed, because exit receives a uchar,
        // while result can be > 255. There are cases in which
        // the overflow can make it return 0 when its not 0 but a
        // larger number.
        if(result > 0) exit(1);
        exit(0);
    }
    else if (pid == -1)
    {
        Logger_Error("There was an error doing a fork to compile the Behaviour.");
        exit(1);
    }

    // From here, no other process but the original one is executing

    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);

    // Get system's output
    std::string str = "";
    const int buff_size = 1024 * 512;
    char buff[buff_size + 1];
    memset((char*)&buff, 0, buff_size + 1);
    while (read(fd[0], buff, buff_size))
    {
        str.append(buff);
        memset(buff, 0, buff_size);
    }
    char *ret = (char*) malloc( str.size() );
    strcpy(ret, (char*) str.c_str());

    // Wait for child and get its ret value
    int result = 1;
    wait(&result);
    result = WEXITSTATUS(result);

    // Close and restore environment
    close(fd[0]);
    dup2(STDIN_FILENO,  old_fd[0]);
    dup2(STDOUT_FILENO, old_fd[1]);
    dup2(STDERR_FILENO, old_fd[2]);

    // Set output parameters
    success = (result == 0);
    if(ret != nullptr) // Set output var
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
    sharedObjectFilepath += scriptDir + "/" + scriptName + "_" +
                            ".so." + std::to_string(Time::GetNow()) + ".1.1";


    std::string cmd = "";
    cmd += "/usr/bin/g++ -shared ";
    cmd += filepath + " " + options + " -o " + sharedObjectFilepath;

    std::string output = "";
    std::replace(cmd.begin(), cmd.end(), '\n', ' '); // Remove line breaks
    SystemUtils::System(cmd, output, ok);

    if (ok)
    {
        if(output != "")
        {
            Logger_Warn(output);
        }
    }
    else
    {
        if(output != "")
        {
            Logger_Error(output);
        }
        else
        {
            Logger_Error("There was an error compiling the Behaviour...");
        }

        sharedObjectFilepath = "";
    }

    return sharedObjectFilepath;
}

void SystemUtils::CreateDynamicBehaviour(const  std::string &sharedObjectFilepath,
                                         Behaviour **createdBehaviour,
                                         void **openLibrary)
{
    dlerror(); // Clear last error just in case

    // Open library
    *openLibrary = dlopen(sharedObjectFilepath.c_str(), RTLD_NOW);
    if(*openLibrary == nullptr) return;

    // Error Check
    char *err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
        dlclose(*openLibrary);
        *openLibrary = *createdBehaviour = nullptr;
        return;
    }


    // Get the pointer to the CreateDynamically function
    Behaviour* (*createFunction)(SingletonManager*) =
            (Behaviour* (*)(SingletonManager*)) (dlsym(*openLibrary, "CreateDynamically"));

    // Error Check
    err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
        dlclose(*openLibrary);
        *openLibrary = *createdBehaviour = nullptr;
        return;
    }

    // Call it and get the pointer to the created Behaviour
    *createdBehaviour = nullptr;
    if(createFunction != nullptr)
    {
        // Create the Behaviour, passing to it the SingletonManager
        // of this main binary, so it can link it.
        *createdBehaviour = createFunction(SingletonManager::GetInstance());
    }
}

bool SystemUtils::DeleteDynamicBehaviour(Behaviour *b, void *openLibrary)
{
    dlerror(); // Clear last error just in case

    // Error Check
    char *err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
        return false;
    }


    // Get the pointer to the CreateDynamically function
    void (*deleteFunction)(Behaviour*) =
            (void (*)(Behaviour*)) (dlsym(openLibrary, "DeleteDynamically"));

    // Error Check
    err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
        return false;
    }

    if(deleteFunction != nullptr)
    {
        deleteFunction(b);
    }
    else return false;

    return true;
}

void SystemUtils::CloseLibrary(void *library)
{
    dlerror();
    dlclose(library);
    char *err = dlerror();
    if(err != nullptr)
    {
        Logger_Error(err);
    }
}

