#include "IWindow.h"

#include "Application.h"

#ifdef BANG_EDITOR
#include "EditorWindow.h"
#else
#include "GameWindow.h"
#endif


IWindow::IWindow()
{
}

QWidget *IWindow::GetWidgetBelowMouse()
{
    IWindow *instance = IWindow::GetInstance();
    return instance ? instance->_GetWidgetBelowMouse() : nullptr;
}

IWindow *IWindow::GetInstance()
{
    #ifdef BANG_EDITOR
    return EditorWindow::GetInstance();
    #else
    return GameWindow::GetInstance();
    #endif
}

bool IWindow::IsInFront()
{
    return IWindow::GetInstance()->GetMainWindow()->isActiveWindow();
}

QWidget *IWindow::_GetWidgetBelowMouse() const
{
    return m_app ? m_app->widgetAt(QCursor::pos()) : nullptr;
}
