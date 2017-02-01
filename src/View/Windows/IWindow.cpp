#include "IWindow.h"

#include "Application.h"

IWindow *IWindow::m_instance = nullptr;

IWindow::IWindow()
{
    IWindow::m_instance = this;
}

void IWindow::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
}

QWidget *IWindow::GetWidgetBelowMouse()
{
    IWindow *instance = IWindow::GetInstance();
    return instance ? instance->_GetWidgetBelowMouse() : nullptr;
}

IWindow *IWindow::GetInstance()
{
    return m_instance;
}

QWidget *IWindow::_GetWidgetBelowMouse() const
{
    return m_app ? m_app->widgetAt(QCursor::pos()) : nullptr;
}
