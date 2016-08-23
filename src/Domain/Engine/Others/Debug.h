#ifndef DEBUG_H
#define DEBUG_H

#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <iostream>

class Color;
class String;
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class IToString;
class Quaternion;
class Debug
{
private:
    Debug();

    static const String c_logPrefix;
    static const String c_warnPrefix;
    static const String c_errorPrefix;

public:
    static void _Log(const String &str);

    static void _Warn(const String &str);

    static void _Error(const String &str);
};

#define Debug_Log(msg) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Log(log.str()); \
} while (0)

#define Debug_Warn(msg) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Warn(log.str()); \
} while (0)

#define Debug_Error(msg) do{\
    std::ostringstream log;\
    log << msg;\
    log.flush();\
    Debug::_Error(log.str()); \
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
std::ostream& operator<<(std::ostream &log, const String &str);
std::ostream& operator<<(std::ostream &log, const IToString *s);
//

// Templated ostream operators
// List
template <class T>
std::ostream &operator<<(std::ostream &log, const std::list<T> *l)
{
    log << "("; bool first = true;
    for (auto it = l->begin(); it != l->end(); ++it)
    {
        if (!first) log << ", ";
        log << (*it);
        first = false;
    }
    log <<")";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const std::list<T> &l)
{
    return log << (&l);
}

// Vector
template <class T>
std::ostream &operator<<(std::ostream &log, const std::vector<T> *v)
{
    log << "[";
    for (int i = 0; i < v->size(); ++i)
    {
        if (i != 0) log << ", ";
        log << (*v)[i];
    }
    log << "]";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const std::vector<T> &v)
{
    return log << (&v);
}

// Map
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const std::map<T,S> *m)
{
    log << "{";
    for (auto it = m->begin(); it != m->end(); ++it)
    {
        if (it != m->begin()) log << ", ";
        log << (it->first) << ": " << (it->second);
    }
    log << "}";
    return log;
}
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const std::map<T,S> &m)
{
    return log << (&m);
}

#endif // DEBUG_H
