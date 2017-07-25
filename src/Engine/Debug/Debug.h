#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <sstream>
#include <iostream>

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix4.h"
#include "Bang/OStreamOperators.h"

class String;
class IToString;
class Quaternion;
class Debug
{
public:
    static void _Clear();
    static void _Log(const String &str, int line, const String &fileName);
    static void _Warn(const String &str, int line, const String &fileName);
    static void _Error(const String &str, int line, const String &fileName);
    static void _Status(const String &str, float timeInSeconds);

private:
    Debug();

    static const String c_logPrefix;
    static const String c_warnPrefix;
    static const String c_errorPrefix;
    static const String c_statusPrefix;
};

#define Debug_Clear() do{\
    Debug::_Clear(); \
} while (0)

#define Debug_Log(msg) do{\
    std::ostringstream log;\
    log << std::boolalpha << msg;\
    log.flush();\
    Debug::_Log(log.str(), __LINE__, __FILE__); \
} while (0)

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

#define Debug_Error_Unique(msg) do{\
    std::ostringstream log;\
    log << std::boolalpha << msg;\
    log.flush();\
    Debug::_Error(log.str(), __LINE__, __FILE__, true); \
} while (0)

#define Debug_Status(msg, timeInSeconds) do{\
    std::ostringstream log;\
    log << std::boolalpha << msg;\
    log.flush();\
    Debug::_Status(log.str(), timeInSeconds); \
} while (0)

#define ASSERT(assertion) if ( !(assertion) ) {\
    Debug_Error("Assertion '" << #assertion << "' failed!"); }

#endif // DEBUG_H
