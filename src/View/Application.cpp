#include "Application.h"

#ifdef BANG_EDITOR
#include "WindowEventManager.h"
#endif

#include "DragDropManager.h"

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
    #endif

    return QApplication::notify(receiver, e);
}
