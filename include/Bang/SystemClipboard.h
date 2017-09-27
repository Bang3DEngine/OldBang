#ifndef SYSTEMCLIPBOARD_H
#define SYSTEMCLIPBOARD_H

#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class SystemClipboard
{
public:
    static void Set(const String &str);
    static String Get();

    SystemClipboard() = delete;
};

NAMESPACE_BANG_END

#endif // SYSTEMCLIPBOARD_H
