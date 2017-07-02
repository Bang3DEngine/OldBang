#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <QString>
#include <sstream>
#include <iostream>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/OStreamOperators.h"

class String;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class IToString;
class Quaternion;
class Debug
{
public:

    static void DrawLine(const Vector3 &origin,
                         const Vector3 &destiny,
                         const Color &color = Color::Green,
                         float lineWidth = 1.0f,
                         float secsTime = 1.0f,
                         bool depthTest = true);

    static void DrawScreenLine(const Vector2 &origin,
                               const Vector2 &destiny,
                               const Color &color = Color::Green,
                               float lineWidth = 1.0f,
                               float secsTime = 1.0f);

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

#define _Debug_Suffix \
       " (file:'" << __FILE__ << "':" << __LINE__ << ", '" << __FUNCTION__ << \
         "')" << std::endl

#endif // DEBUG_H
