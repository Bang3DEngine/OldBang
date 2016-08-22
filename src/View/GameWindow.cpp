#include "GameWindow.h"

#include "SingletonManager.h"

#include <QLabel>
#include <QLayout>

GameWindow *GameWindow::s_m_win = nullptr;

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
    screen = new Screen(GameWindow::GetInstance()->m_mainWindow);
    screen->setMouseTracking(true);
    screen->setFocusPolicy(Qt::StrongFocus);

    GameWindow::GetInstance()->m_mainWindow->setCentralWidget(screen);
}


void GameWindow::InitFromMainBinary(QMainWindow *window, QApplication *application)
{
    IWindow::InitFromMainBinary(window, application);

    GameWindow::s_m_win = new GameWindow(window);
    SingletonManager::GetInstance()->SetWindowMainSingleton(GameWindow::s_m_win);

    GameWindow::s_m_win->m_mainWindow = window;
    GameWindow::s_m_win->SetupUI();
    GameWindow::s_m_win->m_mainWindow->showMaximized();

    GameWindow::s_m_win->m_app = application;
}

GameWindow *GameWindow::GetInstance()
{
    return static_cast<GameWindow*>(SingletonManager::GetInstance()->GetWindowSingleton());
}

QMainWindow *GameWindow::GetMainWindow() const
{
    return GameWindow::GetInstance()->m_mainWindow;
}

QApplication *GameWindow::GetApplication() const
{
    return GameWindow::GetInstance()->m_app;
}
