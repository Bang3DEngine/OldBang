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

private:
    SystemUtils() {}

};

NAMESPACE_BANG_END

#endif // SYSTEMUTILS_H
