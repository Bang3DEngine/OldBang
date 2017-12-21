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

NAMESPACE_BANG_BEGIN

FORWARD class Library;

class SystemUtils
{
public:

    static void System(const String &command,
                       const List<String> &argsList = {},
                       String *output = nullptr,
                       bool *success = nullptr);

    static void SystemBackground(const String &command,
                                 const List<String> &argsList = {});

    static void CloseLibrary(Library *library);

private:
    SystemUtils() {}

    static void _System(const String &command,
                        const List<String> &argsList,
                        String *output,
                        bool *success,
                        bool background);

};

NAMESPACE_BANG_END

#endif // SYSTEMUTILS_H
