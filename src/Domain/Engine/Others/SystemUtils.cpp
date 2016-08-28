#include "SystemUtils.h"

#include "Behaviour.h"
#include "SingletonManager.h"

String SystemUtils::GetAllProjectObjects()
{
    String cmdGetAllObjects = "";
    cmdGetAllObjects = " find " +                                  // Find recursively
                       Persistence::GetProjectRootPathAbsolute() + // From project root
                       " -type f " +                               // Only files
                       " | grep -E -v \"\\..*/.*\" " +             // Not including hidden dirs
                       " | grep -E -v \"BangPreprocessor\" " +     // Temporal fix with colliding .o's TODO
                       " | grep -E -v \"main\\.o\" " +             // Temporal fix with colliding .o's TODO
                       #ifdef BANG_EDITOR
                       " | grep -E \"objEditor\"" +
                       #else
                       " | grep -E \"objGame\"" +
                       #endif
                       " | grep -E \"\\.o$\"" +                    // Only .o files
                       " | xargs";                                 // Inline

    bool ok = false;
    String objs = "";
    SystemUtils::System(cmdGetAllObjects, &objs, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find object files to compile");
    }
    return objs;
}

String SystemUtils::GetAllProjectSubDirs()
{
    String cmdGetAllSubDirs = "";
    cmdGetAllSubDirs = " find " +                                  // Find recursively
                       Persistence::GetProjectRootPathAbsolute() + // From project root
                       " -type d " +                               // Only directories
                       " | grep -E -v \"\\.\" " +                  // Not including hidden dirs
                       " | xargs";                                 // Inline

    bool ok = false;
    String allSubDirs = "";
    SystemUtils::System(cmdGetAllSubDirs, &allSubDirs, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find include directories to compile.");
    }

    return allSubDirs;
}

String SystemUtils::GetQtIncludes()
{
    String qtIncludeDirs = "";
    String cmdGetQtIncludeDirs = "";
    cmdGetQtIncludeDirs =  "find $(qmake -query QT_INSTALL_HEADERS) -type d";
    cmdGetQtIncludeDirs += " | grep -E \"qt|QT|Qt\"";

    bool ok = false;
    SystemUtils::System(cmdGetQtIncludeDirs, &qtIncludeDirs, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find Qt include directories to compile.");
    }

    return qtIncludeDirs;
}

String SystemUtils::GetQtLibrariesDirs()
{
    String qtLibDirs = "";
    //String cmdGetQtLibDirs = "find $(qmake -query QT_INSTALL_LIBS) -type d " +
    //                              "grep -E \"qt|QT|Qt\"";
    String cmdGetQtLibDirs = "qmake -query QT_INSTALL_LIBS";
    bool ok = false;
    SystemUtils::System(cmdGetQtLibDirs, &qtLibDirs, &ok);
    if (!ok)
    {
        Debug_Error("Error trying to find Qt library directories to compile.");
    }

    return qtLibDirs;
}

void SystemUtils::System(const String &command, String *output, bool *success)
{
    int fd[2];
    int old_fd[3] = {dup(STDIN_FILENO), dup(STDOUT_FILENO), dup(STDERR_FILENO)};
    int pipeResult = pipe(fd);
    if (pipeResult != 0)
    {
        if (output)
        {
            *output = "There has been an error when creating a pipe to execute a System instruction.";
        }

        if (success)
        {
            *success = false;
        }

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

        int result = system(command.ToCString());
        close (fd[1]);

        // This is needed, because exit receives a uchar,
        // while result can be > 255. There are cases in which
        // the overflow can make it return 0 when its not 0 but a
        // larger number.
        if (result > 0) quick_exit(1);
        quick_exit(0);
    }
    else if (pid == -1)
    {
        Debug_Error("There was an error doing a fork to execute a System instruction.");
        quick_exit(1);
    }

    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);

    // Get system's output
    if (output)
    {
        *output = "";
    }

    const int bufferSize = 1024;
    char buff[bufferSize + 1];
    memset((char*) &buff, 0, bufferSize + 1);

    while ( read(fd[0], buff, bufferSize) )
    {
        if (output)
        {
            output->append(buff);
        }

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
    if (success)
    {
        *success = (result == 0);
    }
}

void SystemUtils::SystemBackground(const String &command)
{
    String cmd = command + " &";
    system(cmd.ToCString());
}

String SystemUtils::CompileToSharedObject(const String &filepathFromProjectRoot)
{
    // GET INCLUDES
    // Get all subdirs recursively in a single line, and add -I in front of every path

    String includes = "";
    includes += SystemUtils::GetAllProjectSubDirs();
    includes += " . ";
    includes += SystemUtils::GetQtIncludes();
    StringUtils::RemoveLineBreaks(&includes);
    StringUtils::AddInFrontOfWords("-I", &includes);

    String objs = SystemUtils::GetAllProjectObjects();
    StringUtils::RemoveLineBreaks(&objs);

    String qtLibDirs = SystemUtils::GetQtLibrariesDirs();
    StringUtils::RemoveLineBreaks(&qtLibDirs);
    StringUtils::AddInFrontOfWords("-L", &qtLibDirs);

    // Gather options
    String options = "";
    options += " " + objs  + " ";
    options += " -O2";
    options += " -g ";
    #ifdef BANG_EDITOR
    options += " -D BANG_EDITOR ";
    #endif
    options += " -Wl,--export-dynamic ";
    options += " --std=c++11";
    options += " " + includes + " ";
    options += " -lGLEW -lGL -lpthread ";
    options += " " + qtLibDirs + " ";
    options += " -fPIC"; // Shared linking stuff
    //

    String filepath =
            Persistence::ToAbsolute(filepathFromProjectRoot);
    String scriptDir = Persistence::GetDir(filepath);
    String scriptName = Persistence::GetFileNameWithExtension(filepath);

    // Compile
    String sharedObjectFilepath = "";
    sharedObjectFilepath += scriptDir + "/" + scriptName + ".so." + std::to_string(Time::GetNow()) + ".1.1";

    String cmd = "";
    cmd += "/usr/bin/g++ -shared ";
    cmd += filepath + " " + options + " -o " + sharedObjectFilepath;
    StringUtils::RemoveLineBreaks(&cmd);

    String output = "";
    bool ok = false;
    SystemUtils::System(cmd, &output, &ok);

    if (ok)
    {
        if (output != "")
        {
            Debug_Warn(output);
        }
    }
    else // There has been an error
    {
        if (output != "")
        {
            Debug_Error(output);
        }
        else
        {
            Debug_Error("There was an error compiling the Behaviour...");
        }

        sharedObjectFilepath = "";
    }

    return sharedObjectFilepath;
}

Behaviour* SystemUtils::CreateDynamicBehaviour(QLibrary *openLibrary)
{
    QLibrary *lib = openLibrary;
    if (!lib)
    {
        return nullptr;
    }

    if (lib->isLoaded())
    {
        // Get the pointer to the CreateDynamically function
        Behaviour* (*createFunction)(SingletonManager*) =
                (Behaviour* (*)(SingletonManager*)) (lib->resolve("CreateDynamically"));

        if (createFunction)
        {
            // Call it and get the pointer to the created Behaviour
            if (createFunction)
            {
                // Create the Behaviour, passing to it the SingletonManager
                // of this main binary, so it can link it.
                return createFunction(SingletonManager::GetInstance());
            }
        }
        else
        {
            Debug_Error(lib->errorString());
        }
    }
    return nullptr;
}

bool SystemUtils::DeleteDynamicBehaviour(Behaviour *b, QLibrary *openLibrary)
{
    if (!openLibrary)
    {
        return false;
    }

    // Get the pointer to the CreateDynamically function
    void (*deleteFunction)(Behaviour*) =
            (void (*)(Behaviour*)) (openLibrary->resolve("DeletedDinamically"));

    if (deleteFunction)
    {
        deleteFunction(b);
        return true;
    }
    else
    {
        Debug_Error(openLibrary->errorString().toStdString());
        return false;
    }
}

void SystemUtils::CloseLibrary(QLibrary *library)
{
    if (!library->unload())
    {
        Debug_Error(library->errorString().toStdString());
    }
}

