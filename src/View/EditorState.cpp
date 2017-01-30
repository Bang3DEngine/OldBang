#include "EditorState.h"

#include "EditorWindow.h"
#include "EditorPlayStopFlowController.h"

EditorState::EditorState()
{
}

bool EditorState::IsPlaying()
{
    return EditorPlayStopFlowController::GetInstance()->m_playing;
}

bool EditorState::IsUsingGlobalCoords()
{
    return EditorState::GetInstance()->m_globalCoords;
}

bool EditorState::ShowGizmosEnabled()
{
    if (EditorState::IsShowingGameTab())
    {
        return EditorState::ShowGizmosGameEnabled();
    }
    return EditorState::ShowGizmosSceneEnabled();
}

bool EditorState::ShowGizmosSceneEnabled()
{
    return EditorState::GetInstance()->m_showGizmosScene;
}

bool EditorState::ShowGizmosGameEnabled()
{
    return EditorState::GetInstance()->m_showGizmosGame;
}

EditorState::TransformMode EditorState::GetCurrentTransformMode()
{
    return EditorState::GetInstance()->m_currentTransformMode;
}

bool EditorState::IsShowingGameTab()
{
    EditorWindow *win = EditorWindow::GetInstance();
    return win->tabContainerSceneGame->currentWidget() == win->tabGame;
}

bool EditorState::IsShowingSceneTab()
{
    EditorWindow *win = EditorWindow::GetInstance();
    return win->tabContainerSceneGame->currentWidget() == win->tabScene;
}

EditorState *EditorState::GetInstance()
{
    return EditorWindow::GetInstance()->m_editorState;
}

