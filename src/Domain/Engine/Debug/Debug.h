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
    log << msg;\
    log.flush();\
    Debug::_Log(log.str(), __LINE__, __FILE__); \
} while (0)

#define Debug_Warn(msg) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Warn(log.str(), __LINE__, __FILE__); \
} while (0)

#define Debug_Error(msg) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Error(log.str(), __LINE__, __FILE__); \
} while (0)

#define Debug_Error_Unique(msg) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Error(log.str(), __LINE__, __FILE__, true); \
} while (0)

#define Debug_Status(msg, timeInSeconds) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Status(log.str(), timeInSeconds); \
} while (0)

#define _Debug_Suffix \
       " (file:'" << __FILE__ << "':" << __LINE__ << ", '" << __FUNCTION__ << \
         "')" << std::endl

// ostream operators
#define OPERATOR_OSTREAM_DECL(CLASS) \
std::ostream& operator<<(std::ostream &log, const CLASS &v);
#define OPERATOR_OSTREAM_DEF(CLASS) \
std::ostream& operator<<(std::ostream &log, const CLASS &v) { \
    log << v.ToString(); return log; \
}
OPERATOR_OSTREAM_DECL(Color)
OPERATOR_OSTREAM_DECL(Vector2)
OPERATOR_OSTREAM_DECL(Vector3)
OPERATOR_OSTREAM_DECL(Vector4)
OPERATOR_OSTREAM_DECL(Matrix4)
OPERATOR_OSTREAM_DECL(IToString)
OPERATOR_OSTREAM_DECL(Quaternion)
// TODO: Do the same with Vector, List and Map
std::ostream& operator<<(std::ostream &log, const String &str);
std::ostream& operator<<(std::ostream &log, const QString &str);
std::ostream& operator<<(std::ostream &log, const IToString *s);
//

// Templated ostream operators
// List
template <class T>
std::ostream &operator<<(std::ostream &log, const List<T> *l)
{
    log << "("; bool first = true;
    for (auto it = l->Begin(); it != l->End(); ++it)
    {
        if (!first) log << ", ";
        log << (*it);
        first = false;
    }
    log <<")";
    return log;
}

template <class T, class M>
std::ostream &operator<<(std::ostream &log, const std::pair<T,M>& p)
{
    log << "<";
    log << p.first << ", " << p.second;
    log <<">";
    return log;
}

template <class T>
std::ostream &operator<<(std::ostream &log, const List<T> &l)
{
    return log << (&l);
}

// Vector
template <class T>
std::ostream &operator<<(std::ostream &log, const Array<T> *v)
{
    log << "[";
    for (int i = 0; i < v->Size(); ++i)
    {
        if (i != 0) log << ", ";
        log << (*v)[i];
    }
    log << "]";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const Array<T> &v)
{
    return log << (&v);
}

// Map
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const Map<T,S> *m)
{
    log << "{";
    for (auto it = m->Begin(); it != m->End(); ++it)
    {
        if (it != m->begin()) log << ", ";
        log << (it->first) << ": " << (it->second);
    }
    log << "}";
    return log;
}
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const Map<T,S> &m)
{
    return log << (&m);
}

// Set
template <class T>
std::ostream &operator<<(std::ostream &log, const Set<T> *s)
{
    log << "{";
    for (auto it = s->begin(); it != s->end(); ++it)
    {
        if (it != s->begin()) log << ", ";
        log << (*it);
    }
    log << "}";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const Set<T> &m)
{
    return log << (&m);
}

#endif // DEBUG_H
