#include "EditorApplication.h"

#include "Bang/Console.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/EditorPlayFlow.h"
#include "Bang/DragDropManager.h"
#include "Bang/WindowEventManager.h"

EditorApplication::EditorApplication() : Application()
{

}

EditorApplication::~EditorApplication()
{
    delete m_fileTracker;
}

void EditorApplication::InitManagers()
{
    Application::InitManagers();
    m_fileTracker = new FileTracker();
}

EditorApplication *EditorApplication::GetInstance()
{
    if (!EditorWindow::GetInstance()) { return nullptr; }
    return Object::SCast<EditorApplication>(
                EditorWindow::GetInstance()->GetApplication());
}

bool EditorApplication::notify(QObject *receiver, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress)
    {
        DragDropManager::HandleGlobalMousePress(receiver, e);
    }
    else if (e->type() == QEvent::MouseButtonRelease)
    {
        DragDropManager::HandleGlobalMouseRelease(receiver, e);
    }

    return Application::notify(receiver, e);
}

void EditorApplication::OnMainLoopIterationEnd()
{
    Toolbar::GetInstance()->OnEditorUpdate();
    EditorPlayFlow::GetInstance()->OnFrameFinished();
    Console::GetInstance()->ProcessMessagesQueue();
}
