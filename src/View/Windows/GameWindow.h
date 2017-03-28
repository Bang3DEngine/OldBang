#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QApplication>

#include "Bang/IWindow.h"

class Screen;
class GameWindow : public IWindow
{
private:
    static GameWindow *s_m_win;
    void SetupUI();

public:
    Screen *screen = nullptr;

    GameWindow(QMainWindow *window);

    static void InitFromMainBinary(QMainWindow *window, QApplication *application);

    static GameWindow *GetInstance();
    QMainWindow *GetMainWindow() const override;
    QApplication *GetApplication() const override;
};

#endif // GAMEWINDOW_H
