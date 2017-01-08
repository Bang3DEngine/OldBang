#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "String.h"

class QLibrary;
class Behaviour;
class SystemUtils
{
private:
    SystemUtils() {}

public:

    static String GetAllProjectObjects();
    static String GetAllEngineObjects();

    static String GetAllProjectSubDirs();
    static String GetAllEngineSubDirs();

    static String GetQtIncludes();
    static String GetQtLibrariesDirs();

    /**
     * @brief Executes a system command.
     * @param The command to be executed.
     * @param The output of the command.
     * @param Whether it's been successful or not
     */
    static void System(const String &command,
                       String *output = nullptr,
                       bool *success = nullptr);

    static void SystemBackground(const String &command);

    /**
     * @brief Compiles the passed script into a shared library, whom path is
     * returned.
     * @param filepath of the script to compile. Relative to the project root.
     * @param with the BANG_EDITOR define or without it
     * @return The filepath of the created library
     */
    static String CompileToSharedObject(const String &filepathFromProjectRoot,
                                        bool editorMode = true);

    /**
     * @brief Creates a Behaviour from its QLibrary passed as parameter.
     * This will try to call the function CreateDynamically, loading the library
     * passed as parameter. This method CreateDynamically should be defined
     * in the XXXBehaviour.cpp file, as extern "C".
     * @param The dynamic library of the Behaviour that you must have opened before.
     * @return The created Behaviour.
     */
    static Behaviour* CreateDynamicBehaviour(QLibrary *openLibrary);

    /**
     * @brief Delete dynamic Behaviour from a sharedObject passed as parameter.
     * @param The Behaviour you want to delete
     * @param The library that has been opened for the Behaviour you want to delete.
     * @return Success or not.
     */
    static bool DeleteDynamicBehaviour(Behaviour *b,    // NOT USED FTM
                                       QLibrary *openLibrary);


    static void CloseLibrary(QLibrary *library);
};

#endif // SYSTEMUTILS_H
