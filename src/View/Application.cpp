#include "Application.h"

#ifdef BANG_EDITOR
#include "WindowEventManager.h"
#endif

#include "DragDropManager.h"
#include "ShortcutManager.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
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
        ShortcutManager::OnKeyPressed( Input::Key(ev->key()) );
    }
    else if (e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(e);
        ShortcutManager::OnKeyReleased( Input::Key(ev->key()) );
    }
    #endif

    return QApplication::notify(receiver, e);
}
