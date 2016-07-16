#include "SystemUtils.h"

#include "Behaviour.h"
#include "SingletonManager.h"

std::string SystemUtils::GetAllProjectObjects()
{
    std::string cmdGetAllObjects = "";
    cmdGetAllObjects = " find " +                                  // Find recursively
                       Persistence::GetProjectRootPathAbsolute() + // From project root
                       " -type f " +                               // Only files
                       " | grep -E -v \"\\..*/.*\" " +             // Not including hidden dirs
                       " | grep -E -v \"Preprocessor\" " +         // Temporal fix with colliding .o's TODO
                       " | grep -E -v \"main\\.o\" " +             // Temporal fix with colliding .o's TODO
                       " | grep -E \"objEditor\"" +                    // In objEditor directory
                       " | grep -E \"\\.o$\"" +                    // Only .o files
                       " | xargs";                                 // Inline

    bool ok = false;
    std::string objs = "";
    SystemUtils::System(cmdGetAllObjects, objs, ok);
    if(!ok)
    {
        Logger_Error("Error trying to find object files to compile");
    }
    return objs;
}

std::string SystemUtils::GetAllProjectSubDirs()
{
    std::string cmdGetAllSubDirs = "";
    cmdGetAllSubDirs = " find " +                                  // Find recursively
                       Persistence::GetProjectRootPathAbsolute() + // From project root
                       " -type d " +                               // Only directories
                       " | grep -E -v \"\\.\" " +                  // Not including hidden dirs
                       " | xargs";                                 // Inline

    bool ok = false;
    std::string allSubDirs = "";
    SystemUtils::System(cmdGetAllSubDirs, allSubDirs, ok);
    if(!ok)
    {
        Logger_Error("Error trying to find include directories to compile.");
    }

    return allSubDirs;
}

std::string SystemUtils::GetQtIncludes()
{
    std::string qtIncludeDirs = "";
    std::string cmdGetQtIncludeDirs = "";
    cmdGetQtIncludeDirs =  "find $(qmake -query QT_INSTALL_HEADERS) -type d";
    cmdGetQtIncludeDirs += " | grep -E \"qt|QT|Qt\"";

    bool ok = false;
    SystemUtils::System(cmdGetQtIncludeDirs, qtIncludeDirs, ok);
    if(!ok)
    {
        Logger_Error("Error trying to find Qt include directories to compile.");
    }

    return qtIncludeDirs;
}

std::string SystemUtils::GetQtLibrariesDirs()
{
    std::string qtLibDirs = "";
    //std::string cmdGetQtLibDirs = "find $(qmake -query QT_INSTALL_LIBS) -type d " +
    //                              "grep -E \"qt|QT|Qt\"";
    std::string cmdGetQtLibDirs = "qmake -query QT_INSTALL_LIBS";
    bool ok = false;
    SystemUtils::System(cmdGetQtLibDirs, qtLibDirs, ok);
    if(!ok)
    {
        Logger_Error("Error trying to find Qt library directories to compile.");
    }

    return qtLibDirs;
}

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
        if(result > 0) quick_exit(1);
        quick_exit(0);
    }
    else if (pid == -1)
    {
        Logger_Error("There was an error doing a fork to compile the Behaviour.");
        quick_exit(1);
    }

    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);

    // Get system's output
    output = "";
    const int bufferSize = 1024;
    char buff[bufferSize + 1];
    memset((char*) &buff, 0, bufferSize + 1);

    while ( read(fd[0], buff, bufferSize) )
    {
        output.append(buff);
        memset(buff, 0, bufferSize);
    }

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
}

std::string SystemUtils::CompileToSharedObject(const std::string &filepathFromProjectRoot)
{
    // GET INCLUDES
    // Get all subdirs recursively in a single line, and add -I in front of every path

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
    options += " -Wl,--export-dynamic ";
    options += " --std=c++11";
    options += " " + includes + " ";
    options += " -lGLEW -lGL -lpthread ";
    options += " " + qtLibDirs + " ";
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
    char *err = dlerror();
    if(err)
    {
        Logger_Error(err);
        if (*openLibrary)
        {
            dlclose(*openLibrary);
            *openLibrary = *createdBehaviour = nullptr;
        }
        return;
    }
    NONULL(*openLibrary);

    // Error Check
    err = dlerror();
    if(err )
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
    if(err )
    {
        Logger_Error(err);
        dlclose(*openLibrary);
        *openLibrary = *createdBehaviour = nullptr;
        return;
    }

    // Call it and get the pointer to the created Behaviour
    *createdBehaviour = nullptr;
    if(createFunction )
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
    if(err )
    {
        Logger_Error(err);
        return false;
    }


    // Get the pointer to the CreateDynamically function
    void (*deleteFunction)(Behaviour*) =
            (void (*)(Behaviour*)) (dlsym(openLibrary, "DeleteDynamically"));

    // Error Check
    err = dlerror();
    if(err )
    {
        Logger_Error(err);
        return false;
    }

    if(deleteFunction )
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
    if(err )
    {
        Logger_Error(err);
    }
}

