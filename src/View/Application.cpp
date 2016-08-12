#include "Application.h"

#include "WindowEventManager.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
}

bool Application::notify(QObject *receiver, QEvent *e)
{
    if (e->type() == QEvent::Drop ||
        e->type() == QEvent::MouseButtonRelease)
    {
        WindowEventManager::NotifyDragStopped();
    }

    return QApplication::notify(receiver, e);
}
