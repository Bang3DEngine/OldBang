#include "Bang/Debug.h"

#include "Bang/Path.h"
#include "Bang/String.h"

USING_NAMESPACE_BANG

const String Debug::c_logPrefix    = "[   LOG   ]: ";
const String Debug::c_warnPrefix   = "[ WARNING ]: ";
const String Debug::c_errorPrefix  = "[  ERROR  ]: ";

Debug::Debug() { }


void Debug::_Log(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    ::std::cerr << c_logPrefix << str << " | " <<
                 fileName << "(" << line << ")" <<  ::std::endl;
    ::std::cerr.flush();
}

void Debug::_Warn(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    ::std::cerr << c_warnPrefix << str << " | " <<
                 fileName << "(" << line << ")" << ::std::endl;
    ::std::cerr.flush();
}

void Debug::_Error(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    ::std::cerr << c_errorPrefix << str << " | " <<
                 fileName << "(" << line << ")" << ::std::endl;
    ::std::cerr.flush();
}


