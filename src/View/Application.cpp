#include "Application.h"

#ifdef BANG_EDITOR
#include "WindowMain.h"
#include "WindowEventManager.h"
#endif

#include "DragDropManager.h"
#include "ShortcutManager.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{

}

Application *Application::GetInstance()
{
    return static_cast<Application*>(WindowMain::GetInstance()->GetApplication());
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    #ifdef BANG_EDITOR
    if (e->type() == QEvent::MouseButtonPress)
    {
        DragDropManager::HandleGlobalMousePress(receiver, e);
    }
    else if (e->type() == QEvent::MouseButtonRelease)
    {
        DragDropManager::HandleGlobalMouseRelease(receiver, e);
    }

    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(e);
        if (!ev->isAutoRepeat())
        {
            ShortcutManager::GetInstance()->OnKeyPressed( Input::Key(ev->key()) );
        }
    }
    else if (e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(e);
        if (!ev->isAutoRepeat())
        {
            ShortcutManager::GetInstance()->OnKeyReleased( Input::Key(ev->key()) );
        }
    }

    if (e->type() == QEvent::Shortcut)
    {
        ShortcutManager::GetInstance()->Clear();
    }
    #endif

    return QApplication::notify(receiver, e);
}
