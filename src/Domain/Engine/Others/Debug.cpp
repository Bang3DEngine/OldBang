#include "Debug.h"

#include "Color.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "IToString.h"
#include "Quaternion.h"

#ifdef BANG_EDITOR
#include "ListLogger.h"
#endif

const String Debug::c_logPrefix   = "[   LOG   ]: ";
const String Debug::c_warnPrefix  = "[ WARNING ]: ";
const String Debug::c_errorPrefix = "[  ERROR  ]: ";

Debug::Debug()
{
}

void Debug::_Log(const String &str)
{
    std::cerr << c_logPrefix << str << std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    ListLogger::AddLog(str);
    #endif
}

void Debug::_Warn(const String &str)
{
    std::cerr << c_warnPrefix << str << std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    ListLogger::AddWarn(str);
    #endif
}

void Debug::_Error(const String &str)
{
    std::cerr << c_errorPrefix << str << std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    ListLogger::AddError(str);
    #endif
}

// ostream operators
OPERATOR_OSTREAM_DEF(Color)
OPERATOR_OSTREAM_DEF(Vector2)
OPERATOR_OSTREAM_DEF(Vector3)
OPERATOR_OSTREAM_DEF(Vector4)
OPERATOR_OSTREAM_DEF(Matrix4)
OPERATOR_OSTREAM_DEF(IToString)
OPERATOR_OSTREAM_DEF(Quaternion)

std::ostream& operator<<(std::ostream &log, const String &str)
{
    log << str.ToCString(); return log;
}

std::ostream &operator<<(std::ostream &log, const IToString *s)
{
    if (!s)
    {
        log << "Null";
    }
    else
    {
        log << s->ToString();
    }
    return log;
}
//


std::ostream &operator<<(std::ostream &log, const QString &str)
{
    log << str.toStdString(); return log;
}
