#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <sstream>
#include <iostream>

#include "Bang/String.h"
#include "Bang/StreamOperators.h"

class Debug
{
public:
    static void _Log(const String &str, int line, const String &fileName);
    static void _Warn(const String &str, int line, const String &fileName);
    static void _Error(const String &str, int line, const String &fileName);

private:
    Debug();

    static const String c_logPrefix;
    static const String c_warnPrefix;
    static const String c_errorPrefix;
};

#define Debug_Log(msg) do{\
    std::ostringstream log;\
    log << std::boolalpha << msg;\
    log.flush();\
    Debug::_Log(log.str(), __LINE__, __FILE__); \
} while (0)

#define Debug_Peek(varName) Debug_Log(#varName << ": " << (varName))

#define Debug_Warn(msg) do{\
    std::ostringstream log;\
    log << std::boolalpha << msg;\
    log.flush();\
    Debug::_Warn(log.str(), __LINE__, __FILE__); \
} while (0)

#define Debug_Error(msg) do{\
    std::ostringstream log;\
    log << std::boolalpha << msg;\
    log.flush();\
    Debug::_Error(log.str(), __LINE__, __FILE__); \
} while (0)

#endif // DEBUG_H
