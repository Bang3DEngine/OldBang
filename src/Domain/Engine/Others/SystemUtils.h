#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Bang/List.h"
#include "Bang/Path.h"
#include "Bang/Flags.h"
#include "Bang/String.h"

class QLibrary;
class Behaviour;
class SystemUtils
{

public:
    enum CompilationFlag
    {
        None                  = 0,
        ProduceSharedLib      = 1,
        AddEngineObjectFiles  = 2,
        AddProjectObjectFiles = 4,
        ForGame               = 8,
        AddAssetsIncludeDirs  = 16,

        Default = AddProjectObjectFiles | AddEngineObjectFiles
    };
    CREATE_FLAGS(CompilationFlags, CompilationFlag);

    static List<Path> GetAllEngineObjects(bool editorMode);

    static List<Path> GetAllProjectSubDirs();
    static List<Path> GetAllEngineSubDirs();

    static List<Path> GetQtIncludes();
    static List<Path> GetQtLibrariesDirs();

    /**
     * @brief Executes a system command.
     * @param The command to be executed.
     * @param The output of the command.
     * @param Whether it's been successful or not
     */
    static void System(const String &command,
                       const List<String> &argsList = {},
                       String *output = nullptr,
                       bool *success = nullptr);

    static void SystemBackground(const String &command,
                                 const List<String> &argsList = {});

    static void Compile(List<Path> &sourceFilesList,
                        const Path &outputLibFilepath,
                        CompilationFlags clFlags = CompilationFlag::Default,
                        bool *success = nullptr,
                        String *output = nullptr);

    static void CloseLibrary(QLibrary *library);

private:
    SystemUtils() {}

    static void _System(const String &command,
                        const List<String> &argsList,
                        String *output,
                        bool *success,
                        bool background);

    static List<String> ToStringList(const List<Path> &paths);

};

#endif // SYSTEMUTILS_H
