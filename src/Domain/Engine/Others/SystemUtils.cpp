#include "SystemUtils.h"

#include "Behaviour.h"
#include "SingletonManager.h"

// TODO: Fix System success variable (it does not do what it's supposed to do)
void SystemUtils::AddInFrontOfWords(std::string particle, std::string *str)
{
    std::string &phrase = *str;
    if(phrase.length() > 0 && phrase[0] != ' ')
    {
        phrase.insert(0, particle);
    }

    for(int i = 0; i < phrase.length() -1; ++i)
    {
        if(phrase[i] == ' ' && phrase[i+1] != ' ')
        {
            phrase.insert(i+1, particle);
            i += 2; // Sorry
        }
    }
}

void SystemUtils::RemoveLineBreaks(std::string *str)
{
    std::replace(str->begin(), str->end(), '\n', ' '); // Remove line breaks
}

std::string SystemUtils::GetAllProjectObjects(const std::string &filepathFromProjectRoot)
{
    std::string cmdGetAllObjects = "";
    cmdGetAllObjects = " find " +                                  // Find recursively
                       Persistence::GetProjectRootPathAbsolute() + // From project root
                       " -type f " +                               // Only files
                       " | grep -E -v \"\\..*/.*\" " +             // Not including hidden dirs
                       " | grep -E -v \"Preprocessor\" " +         // Temporal fix with colliding .o's TODO
                       " | grep -E -v \"main\\.o\" " +             // Temporal fix with colliding .o's TODO
                       " | grep -E \"\\.o$\"" +                    // Only .o files
                       " | xargs";                                 // Inline

    bool ok = false;
    std::string objs = "";
    SystemUtils::System(cmdGetAllObjects, objs, ok);
    if(!ok)
    {
        Logger_Error("Error trying to find object files to compile " <<
                     filepathFromProjectRoot);
    }
    return objs;
}

std::string SystemUtils::GetAllProjectSubDirs(const std::string &filepathFromProjectRoot)
{
    // It sometimes gets stuck while reading, I guess the pipe isn't being closed.
    // TODO: Improve this, ftm I will assume that when we have a bit read, we can go on...
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
        Logger_Error("Error trying to find include directories to compile " <<
                     filepathFromProjectRoot);
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
    includes += GetAllProjectSubDirs(filepathFromProjectRoot);
    includes += " . ";
    #ifdef BANG_EDITOR
    includes += GetQtIncludes();
    #endif
    RemoveLineBreaks(&includes);
    AddInFrontOfWords("-I", &includes);

    std::string objs = GetAllProjectObjects(filepathFromProjectRoot);
    RemoveLineBreaks(&objs);

    std::string qtLibDirs = GetQtLibrariesDirs();
    RemoveLineBreaks(&qtLibDirs);
    AddInFrontOfWords("-L", &qtLibDirs);

    // Gather options
    std::string options = "";
    options += " " + objs  + " ";
    options += " -O1";
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

    std::string output = "";
    RemoveLineBreaks(&cmd);

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

