#ifndef LOGGER_H
#define LOGGER_H

#include "Bang.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "glm/glm.hpp"

#include "Time.h"
#include "IToString.h"

#ifdef BANG_EDITOR
#include "ListLogger.h"
#endif

//OSTREAM OPERATORS //////////////////////////////////
std::ostream& operator<<(std::ostream &log, const std::string &str);
std::ostream& operator<<(std::ostream &log, const glm::vec2 &v);
std::ostream& operator<<(std::ostream &log, const Vector3 &v);
std::ostream& operator<<(std::ostream &log, const glm::vec4 &v);
std::ostream& operator<<(std::ostream &log, const Quaternion &q);
std::ostream& operator<<(std::ostream &log, const Matrix4 &v);
std::ostream &operator<<(std::ostream &log, const IToString *s);
std::ostream &operator<<(std::ostream &log, const IToString &s);

template <class T>
std::ostream &operator<<(std::ostream &log, const std::list<T> *l)
{
    log << "{"; bool first = true;
    for(auto it = l->begin(); it != l->end(); ++it)
    {
        if(!first) log << ", ";
        log << (*it);
        first = false;
    }
    log <<"}";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const std::list<T> &l)
{
    return log << (&l);
}
template <class T>
std::ostream &operator<<(std::ostream &log, const std::vector<T> *v)
{
    log << "{";
    for(int i = 0; i < v->size(); ++i)
    {
        if(i != 0) log << ", ";
        log << (*v)[i];
    }
    log << "}";
    return log;
}
template <class T>
std::ostream &operator<<(std::ostream &log, const std::vector<T> &v)
{
    return log << (&v);
}
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const std::map<T,S> *m)
{
    log << "{";
    for(auto it = m->begin(); it != m->end(); ++it)
    {
        if(it != m->begin()) log << ", ";
        log << (it->first) << ": " << (it->second);
    }
    log << ")";
    return log;
}
template <class T, class S>
std::ostream &operator<<(std::ostream &log, const std::map<T,S> &m)
{
    return log << (&m);
}



#define _Logger_Suffix \
       " (file:'" << __FILE__ << "':" << __LINE__ << ", '" << __FUNCTION__ << \
         "')" << std::endl

#ifdef BANG_EDITOR
    #define _Log_To_ListLogger(x)   ListLogger::AddLog(x);
    #define _Warn_To_ListLogger(x)  ListLogger::AddWarn(x);
    #define _Error_To_ListLogger(x) ListLogger::AddError(x);
    #define _Verbose_To_ListLogger(x) do{}while(0);
#else
    #define _Log_To_ListLogger(x) do{}while(0);
    #define _Warn_To_ListLogger(x) do{}while(0);
    #define _Error_To_ListLogger(x) do{}while(0);
    #define _Verbose_To_ListLogger(x) do{}while(0);
#endif

#define Logger_Log(x) do{\
    std::ostringstream log;\
    log << x;\
    std::cerr << "[   LOG   ]: " << log.str() << std::endl;\
    std::cerr.flush();\
    log.flush();\
    _Log_To_ListLogger(log.str()); \
} while(0)

#define Logger_Warn(x) do{\
    std::ostringstream log;\
    log << x;\
    std::cerr << "[ WARNING ]: " << log.str() << _Logger_Suffix;\
    std::cerr.flush();\
    log.flush();\
    _Warn_To_ListLogger(log.str()); \
} while(0)

#define Logger_GetString(strVar, x) do{\
    std::ostringstream log;\
    log << x;\
    strVar = log.str(); \
} while(0)

#define Logger_Error(x) do{\
    std::ostringstream log;\
    log << x;\
    std::cerr << "[  ERROR  ]: " << log.str() << _Logger_Suffix;\
    std::cerr.flush();\
    log.flush();\
    _Error_To_ListLogger(log.str()); \
} while(0)

#define Logger_Verbose(x) do{\
    std::ostringstream log;\
    log << x;\
    /*std::cerr << "[ VERBOSE ]: " << log.str() << _Logger_Suffix;*/\
    /*std::cerr.flush();*/\
    log.flush();\
    _Verbose_To_ListLogger(log.str()); \
} while(0)

#define Assert(mustBeTrue, message, returnAction) do{\
    if(!(mustBeTrue)) {\
        Logger_Error("Assertion failed: " << message);\
        returnAction;\
    }\
} while(0)

#endif //LOGGER_H
