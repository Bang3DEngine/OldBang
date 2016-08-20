#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QApplication>

#include "Bang.h"

#include "Screen.h"
#include "Logger.h"

#include "IWindow.h"

#include "WindowMainAutoGenerated.h"

/**
 *  This will be a singleton shared amongst ALL *.o and *.so
 *  Cross-library shared singleton
 */
class QApplication;
class WindowMain : public Ui_WindowMain
                  ,public IWindow
{

private:
    WindowMain();

    static WindowMain *s_m_win;

public:

    /**
     * @brief This must be called by main() in the mainBinary.
     * This method will register the WindowMain singleton into the
     * SingletonManager, for later use in shared libraries.
     * @return The created Singleton,
     */
    static void InitFromMainBinary(QMainWindow *window, QApplication *app);

    static WindowMain *GetInstance();
    QMainWindow *GetMainWindow() const override;
    QApplication *GetApplication() const override;

};

#endif // WINDOWMAIN_H
