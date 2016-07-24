#ifndef IWINDOW_H
#define IWINDOW_H

#include <QApplication>
#include <QMainWindow>

class IWindow
{
protected:
    QMainWindow *m_mainWindow = nullptr;
    QApplication *m_app = nullptr;

    IWindow();

public:

    static void InitFromMainBinary(QMainWindow *window, QApplication *application);

    static IWindow *GetInstance();
    virtual QMainWindow *GetMainWindow() const = 0;
    virtual QApplication *GetApplication() const = 0;
};

#endif // IWINDOW_H
