#include "GameWindow.h"

#include "SingletonManager.h"

#include <QLabel>
#include <QLayout>

GameWindow *GameWindow::s_p_win = nullptr;

GameWindow::GameWindow(QMainWindow *window)
{
    if (window->objectName().isEmpty())
        window->setObjectName(QString::fromUtf8("WindowMain"));
    window->setWindowModality(Qt::NonModal);
    window->resize(1008, 594);
    window->setMouseTracking(true);
    window->setFocusPolicy(Qt::StrongFocus);
}

void GameWindow::SetupUI()
{
    canvas = new Canvas(GameWindow::GetInstance()->p_mainWindow);
    canvas->setMouseTracking(true);
    canvas->setFocusPolicy(Qt::StrongFocus);

    GameWindow::GetInstance()->p_mainWindow->setCentralWidget(canvas);
}


void GameWindow::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    GameWindow::s_p_win = new GameWindow(window);
    SingletonManager::GetInstance()->SetWindowMainSingleton(GameWindow::s_p_win);

    GameWindow::s_p_win->p_mainWindow = window;
    GameWindow::s_p_win->SetupUI();
    GameWindow::s_p_win->p_mainWindow->showMaximized();

    GameWindow::s_p_win->p_app = application;
}

GameWindow *GameWindow::GetInstance()
{
    return static_cast<GameWindow*>(SingletonManager::GetInstance()->GetWindowSingleton());
}

QMainWindow *GameWindow::GetMainWindow() const
{
    return GameWindow::GetInstance()->p_mainWindow;
}

QApplication *GameWindow::GetApplication() const
{
    return GameWindow::GetInstance()->p_app;
}
