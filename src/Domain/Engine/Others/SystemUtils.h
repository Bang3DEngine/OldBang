#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Bang/List.h"
#include "Bang/String.h"

class QLibrary;
class Behaviour;
class SystemUtils
{

public:
    enum CompilationFlags
    {
        None                  = 0,
        ProduceSharedLib      = 1,
        AddEngineObjectFiles  = 2,
        AddProjectObjectFiles = 4,
        ForGame               = 8,
        AddAssetsIncludeDirs  = 16,
        Default = AddProjectObjectFiles | AddEngineObjectFiles
    };

    static List<String> GetAllEngineObjects(bool editorMode);

    static List<String> GetAllProjectSubDirs();
    static List<String> GetAllEngineSubDirs();

    static List<String> GetQtIncludes();
    static List<String> GetQtLibrariesDirs();

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

    static void Compile(List<String> &sourceFilesList,
                        const String &outputLibFilepath,
                        CompilationFlags clFlags = CompilationFlags::Default,
                        bool *success = nullptr, String *output = nullptr);

    static void CloseLibrary(QLibrary *library);

private:
    SystemUtils() {}

    static void _System(const String &command,
                        const List<String> &argsList,
                        String *output,
                        bool *success,
                        bool background);

};

inline SystemUtils::CompilationFlags operator|(SystemUtils::CompilationFlags a,
                                               SystemUtils::CompilationFlags b)
{
    return static_cast<SystemUtils::CompilationFlags>(static_cast<int>(a) |
                                                      static_cast<int>(b));
}

inline SystemUtils::CompilationFlags operator^(SystemUtils::CompilationFlags a,
                                               SystemUtils::CompilationFlags b)
{
    return static_cast<SystemUtils::CompilationFlags>(static_cast<int>(a) ^
                                                      static_cast<int>(b));
}

#endif // SYSTEMUTILS_H
