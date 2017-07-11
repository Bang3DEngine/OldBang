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

    static void CloseLibrary(QLibrary *library);

private:
    SystemUtils() {}

    static void _System(const String &command,
                        const List<String> &argsList,
                        String *output,
                        bool *success,
                        bool background);

};

#endif // SYSTEMUTILS_H