#include "Debug.h"

#include "Debug.h"
#include "Scene.h"
#include "Color.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "IToString.h"
#include "Quaternion.h"
#include "SceneManager.h"

#ifdef BANG_EDITOR
#include "ListLogger.h"
#include "EditorScene.h"
#include "EditorWindow.h"
#include "EditorDebugGameObject.h"
#endif

const String Debug::c_logPrefix    = "[   LOG   ]: ";
const String Debug::c_warnPrefix   = "[ WARNING ]: ";
const String Debug::c_errorPrefix  = "[  ERROR  ]: ";
const String Debug::c_statusPrefix = "[  STATUS ]: ";

Debug::Debug()
{
}

void Debug::DrawLine(const Vector3 &origin, const Vector3 &destiny, const Color &color,
                     float lineWidth, float secsTime, bool depthTest)
{
    #ifdef BANG_EDITOR
    Scene *s = SceneManager::GetActiveScene(); ASSERT(s);
    EditorScene *es = dynamic_cast<EditorScene*>(s); ASSERT(es);
    EditorDebugGameObject *edgo = es->m_debugGameObject; ASSERT(edgo);
    edgo->DrawLine(origin, destiny, color, lineWidth, secsTime, depthTest);
    #endif
}

void Debug::DrawScreenLine(const Vector2 &origin, const Vector2 &destiny,
                           const Color &color, float lineWidth, float secsTime)
{
    #ifdef BANG_EDITOR
    Scene *s = SceneManager::GetActiveScene(); ASSERT(s);
    EditorScene *es = dynamic_cast<EditorScene*>(s); ASSERT(es);
    EditorDebugGameObject *edgo = es->m_debugGameObject; ASSERT(edgo);
    edgo->DrawScreenLine(origin, destiny, color, lineWidth, secsTime);
#endif
}

void Debug::_Clear()
{
    #ifdef BANG_EDITOR
    ListLogger::Clear();
    #endif
}

void Debug::_Status(const String &str, float timeInSeconds)
{
    std::cerr << c_statusPrefix << str << std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    EditorWindow *win = EditorWindow::GetInstance();
    win->statusbar->showMessage( str.ToQString(), timeInSeconds * 1000 );
    #endif
}

void Debug::_Log(const String &str, int line, const String &filePath)
{
    String fileName = Persistence::GetFileNameWithExtension(filePath);
    std::cerr << c_logPrefix << str << " | " <<
                 fileName << "(" << line << ")" <<  std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    ListLogger::AddLog(str, line, fileName);
    #endif
}

void Debug::_Warn(const String &str, int line, const String &filePath)
{
    String fileName = Persistence::GetFileNameWithExtension(filePath);
    std::cerr << c_warnPrefix << str << " | " <<
                 fileName << "(" << line << ")" << std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    ListLogger::AddWarn(str, line, fileName);
    #endif
}

void Debug::_Error(const String &str, int line, const String &filePath,
                   bool uniqueMessage)
{
    String fileName = Persistence::GetFileNameWithExtension(filePath);
    std::cerr << c_errorPrefix << str << " | " <<
                 fileName << "(" << line << ")" << std::endl;
    std::cerr.flush();
    #ifdef BANG_EDITOR
    ListLogger::AddError(str, line, fileName);
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
