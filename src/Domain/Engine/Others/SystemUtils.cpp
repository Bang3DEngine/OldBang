#include "SystemUtils.h"

#include <QProcess>
#include <QLibrary>

#include "Behaviour.h"
#include "Persistence.h"
#include "BehaviourHolder.h"
#include "SingletonManager.h"

String SystemUtils::GetAllEngineObjects(bool editorMode)
{
    String subdir = editorMode ? "/bin/objEditor" : "/bin/objGame";
    List<String> files =
            Persistence::GetFiles(Persistence::GetEngineRootAbs() + subdir,
                                  true, {"*.o"});
    return String::Join(files, " ");
}

String SystemUtils::GetAllProjectSubDirs()
{
    List<String> subdirs =
            Persistence::GetSubDirectories(Persistence::GetProjectRootAbs(), true);
    subdirs.PushFront(Persistence::GetProjectRootAbs());
    return String::Join(subdirs, " ");
}

String SystemUtils::GetAllEngineSubDirs()
{
    List<String> subdirs =
            Persistence::GetSubDirectories(Persistence::GetEngineRootAbs(), true);
    subdirs.PushFront(Persistence::GetEngineRootAbs());
    return String::Join(subdirs, " ");
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
    /*
    List<String> args = command.Split(' ').ToList();
    args.PopFront();
    QStringList argsList;
    for (const String &arg : args)
    {
        argsList.push_back(arg.ToQString());
    }

    QProcess process;
    process.start(command.ToQString(), argsList.ToQString());
    bool ok = process.waitForFinished(999999);
    process.setReadChannel(QProcess::ProcessChannel::StandardOutput);
    QString out( process.readAll() );
    if (output) { *output = String(out) ; }
    if (success) { *success = ok; }
    /*/
    int fd[2];
    int old_fd[3] = {dup(STDIN_FILENO), dup(STDOUT_FILENO), dup(STDERR_FILENO)};
    int pipeResult = pipe(fd);
    if (pipeResult != 0)
    {
        if (output)
        {
            *output =
                 "Error when creating a pipe to execute a System instruction.";
        }
        if (success) { *success = false; }
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
        Debug_Error(
           "There was an error doing a fork to execute a System instruction.");
        quick_exit(1);
    }

    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);

    // Get system's output
    if (output) { *output = ""; }

    const int bufferSize = 16384;
    char buff[bufferSize + 1];
    memset((char*) &buff, 0, bufferSize + 1);
    while ( read(fd[0], buff, bufferSize) )
    {
        if (output) { output->append(buff); }
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
    if (success) { *success = (result == 0); }
}

void SystemUtils::SystemBackground(const String &command)
{
    String cmd = command + " &";
    system(cmd.ToCString());
}

void SystemUtils::Compile(List<String> &sourceFilesList,
                          const String &outputLibFilepath,
                          CompilationFlags clFlags,
                          bool *success, String *output)
{
    typedef CompilationFlags CLFlags;
    bool editorMode = (clFlags & CLFlags::ForGame) == 0;

    String includes = " . " + SystemUtils::GetAllProjectSubDirs() + " " +
                              SystemUtils::GetAllEngineSubDirs()  + " " +
                              SystemUtils::GetQtIncludes()        + " ";
    includes.ReplaceInSitu("\n", " ");
    includes = includes.AddInFrontOfWords("-I");

    String objs = "";
    if ((clFlags & CLFlags::AddProjectObjectFiles) > 0)
    {
        List<String> objFiles =
                Persistence::GetFiles(Persistence::GetDir(outputLibFilepath),
                                      true, {"*.o"});
        objs += String::Join(objFiles, " ");
        objs += " ";
    }
    bool addEngineObjectFiles = (clFlags & CLFlags::AddEngineObjectFiles) > 0;
    objs += addEngineObjectFiles ?
                (SystemUtils::GetAllEngineObjects(editorMode) + " ") : "";

    String qtLibDirs = SystemUtils::GetQtLibrariesDirs();
    qtLibDirs.ReplaceInSitu("\n", " ");
    qtLibDirs = qtLibDirs.AddInFrontOfWords("-L");

    String options = " " + objs  + " -O0 -g ";
    if (editorMode) { options += " -DBANG_EDITOR "; }
    options += " -Wl,--export-dynamic --std=c++11 " + includes +
               " -lGLEW -lGL -lpthread " + qtLibDirs + " -fPIC";

    String sourcesStr = " " + String::Join(sourceFilesList, " ") + " ";

    String libsDir = BehaviourManager::GetCurrentLibsDir();
    Persistence::CreateDirectory(libsDir);

    bool produceSharedLib = (clFlags & CLFlags::ProduceSharedLib) > 0;
    String sharedOpt = (produceSharedLib ? "-shared" : "-c");
    String cmd = "/usr/bin/g++ " + sharedOpt + " "
                 + sourcesStr + " " + options
                 + " -o " + outputLibFilepath;
    cmd.ReplaceInSitu("\n", " ");

    //Debug_Log("cmd: " << cmd);
    SystemUtils::System(cmd, output, success);
    //Debug_Log("output: " << *output);
}

void SystemUtils::CloseLibrary(QLibrary *library)
{
    if (!library->unload())
    {
        Debug_Error(library->errorString());
    }
}

