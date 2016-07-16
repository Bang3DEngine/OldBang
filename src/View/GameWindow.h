#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QApplication>

#include "Bang.h"

#include "IWindow.h"
#include "Canvas.h"

class GameWindow : public IWindow
{
private:
    static GameWindow *s_p_win;
    void SetupUI();

public:
    Canvas *canvas = nullptr;

    GameWindow(QMainWindow *window);

    static void InitFromMainBinary(QMainWindow *window, QApplication *application);

    static GameWindow *GetInstance();
    QMainWindow *GetMainWindow() const override;
    QApplication *GetApplication() const override;
};

#endif // GAMEWINDOW_H
