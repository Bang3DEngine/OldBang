#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "Bang/WinUndef.h"

#include "Bang/IWindow.h"

class Screen;
class GameWindow : public IWindow
{
public:
    GameWindow(QMainWindow *window);

    static void InitFromMainBinary(QMainWindow *window, QApplication *application);

    static GameWindow *GetInstance();
    Screen *GetScreen() const override;
    QMainWindow *GetMainWindow() const override;
    QApplication *GetApplication() const override;

private:
    Screen *m_screen = nullptr;

    static GameWindow *s_m_win;
    void SetupUI();
};

#endif // GAMEWINDOW_H
