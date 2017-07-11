#ifndef IWINDOW_H
#define IWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include "Bang/WinUndef.h"

class Screen;
class IWindow
{
public:
    static QWidget* GetWidgetBelowMouse();
    static IWindow* GetInstance();

    virtual Screen *GetScreen() const = 0;
    virtual QMainWindow *GetMainWindow() const = 0;
    virtual QApplication *GetApplication() const = 0;

    static bool IsInFront();

protected:
    QMainWindow  *m_mainWindow = nullptr;
    QApplication *m_app = nullptr;

    IWindow();
    QWidget* _GetWidgetBelowMouse() const;
};

#endif // IWINDOW_H