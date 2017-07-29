#include "Bang/Debug.h"

#include "Bang/Path.h"
#include "Bang/Scene.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Matrix4.h"
#include "Bang/IToString.h"
#include "Bang/Quaternion.h"

const String Debug::c_logPrefix    = "[   LOG   ]: ";
const String Debug::c_warnPrefix   = "[ WARNING ]: ";
const String Debug::c_errorPrefix  = "[  ERROR  ]: ";
const String Debug::c_statusPrefix = "[  STATUS ]: ";

Debug::Debug() { }

void Debug::_Clear()
{
}

void Debug::_Status(const String &str, float timeInSeconds)
{
    std::cerr << c_statusPrefix << str << std::endl;
    std::cerr.flush();
}

void Debug::_Log(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    std::cerr << c_logPrefix << str << " | " <<
                 fileName << "(" << line << ")" <<  std::endl;
    std::cerr.flush();
}

void Debug::_Warn(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    std::cerr << c_warnPrefix << str << " | " <<
                 fileName << "(" << line << ")" << std::endl;
    std::cerr.flush();
}

void Debug::_Error(const String &str, int line, const String &filePath)
{
    String fileName = Path(filePath).GetNameExt();
    std::cerr << c_errorPrefix << str << " | " <<
                 fileName << "(" << line << ")" << std::endl;
    std::cerr.flush();
}
