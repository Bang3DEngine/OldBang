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
    static String GetAllEngineObjects(bool editorMode);

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

    static void CloseLibrary(QLibrary *library);
};

#endif // SYSTEMUTILS_H
