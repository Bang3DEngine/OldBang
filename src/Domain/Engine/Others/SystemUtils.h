#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <string>

class Behaviour;
class SystemUtils
{
private:
    SystemUtils() {}

public:

    /**
     * @brief Executes a system command.
     * @param The command to be executed.
     * @param The output of the command.
     * @param Whether it's been successful or not
     */
    static void System(const std::string &command, std::string &output, bool &success);

    /**
     * @brief Compiles the passed script into a shared library, whom path is
     * returned.
     * @param filepath of the script to compile. Relative to the project root.
     * @return The filepath of the created library
     */
    static std::string CompileToSharedObject(const std::string &filepathFromProjectRoot);

    /**
     * @brief Creates a Behaviour from a sharedObject passed as parameter.
     * This will try to call the function CreateDynamically, loading the library
     * passed as parameter. This method CreateDynamically should be defined
     * in the XXXBehaviour.cpp file, as extern "C".
     * @param The filepath to the sharedObject
     * @param The created Behaviour. If error, nullptr.
     * @param The library that has been opened. You must keep this to close the library
     * later with CloseLibrary. If error, nullptr.
     * @return The created Behaviour.
     */
    static void CreateDynamicBehaviour(const std::string &sharedObjectFilepath,
                                       Behaviour **createdBehaviour,
                                       void **openLibrary);

    /**
     * @brief Delete dynamic Behaviour from a sharedObject passed as parameter.
     * @param The Behaviour you want to delete
     * @param The library that has been opened for the Behaviour you want to delete.
     * @return Success or not.
     */
    static bool DeleteDynamicBehaviour(Behaviour *b,    // NOT USED FTM
                                       void *openLibrary);


    static void CloseLibrary(void *library);
};

#endif // SYSTEMUTILS_H
