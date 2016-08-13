#include "Application.h"

#ifdef BANG_EDITOR
#include "WindowEventManager.h"
#endif

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    #ifdef BANG_EDITOR
    if (e->type() == QEvent::Drop ||
        e->type() == QEvent::MouseButtonRelease)
    {
        WindowEventManager::NotifyDragStopped();
    }
    #endif

    return QApplication::notify(receiver, e);
}
