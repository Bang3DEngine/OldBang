#include "Bang/EditorState.h"

#include "Bang/EditorWindow.h"
#include "Bang/EditorPlayFlow.h"

EditorState::EditorState()
{
}

bool EditorState::IsPlaying()
{
    return EditorState::GetPlayState() == PlayState::Playing;
}

bool EditorState::IsPaused()
{
    return EditorState::GetPlayState() == PlayState::Paused;
}

bool EditorState::IsStopped()
{
    return EditorState::GetPlayState() == PlayState::Stopped;
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

EditorState::PlayState EditorState::GetPlayState()
{
    return EditorState::GetInstance()->m_playState;
}

void EditorState::SetTransformMode(EditorState::TransformMode transformMode)
{
    Toolbar::GetInstance()->SetTransformMode(transformMode);
}

EditorState *EditorState::GetInstance()
{
    return EditorWindow::GetInstance()->m_editorState;
}

void EditorState::SetPlayingState(EditorState::PlayState playingState)
{
    EditorState::GetInstance()->m_playState = playingState;
}

