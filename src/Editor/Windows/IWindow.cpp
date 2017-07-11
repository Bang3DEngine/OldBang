#include "Bang/IWindow.h"

#include "Bang/SingletonManager.h"

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
    return SingletonManager::Get<IWindow>();
}

bool IWindow::IsInFront()
{
    return IWindow::GetInstance()->GetMainWindow()->isActiveWindow();
}

QWidget *IWindow::_GetWidgetBelowMouse() const
{
    return m_app ? m_app->widgetAt(QCursor::pos()) : nullptr;
}
