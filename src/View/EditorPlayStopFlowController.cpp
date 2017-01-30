#include "EditorPlayStopFlowController.h"

#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "ListLogger.h"
#include "Application.h"
#include "EditorScene.h"
#include "EditorWindow.h"
#include "SceneManager.h"
#include "EditorCamera.h"
#include "BehaviourManager.h"


EditorPlayStopFlowController::EditorPlayStopFlowController()
{
}

bool EditorPlayStopFlowController::IsPlaying()
{
    return EditorPlayStopFlowController::GetInstance()->m_playing;
}

void EditorPlayStopFlowController::OnPlayClicked()
{
    EditorPlayStopFlowController::GetInstance()->PlayScene();
}

void EditorPlayStopFlowController::OnStopClicked()
{
    EditorPlayStopFlowController::GetInstance()->StopScene();
}

EditorPlayStopFlowController *EditorPlayStopFlowController::GetInstance()
{
    return EditorWindow::GetInstance()->m_playStopController;
}

void EditorPlayStopFlowController::PlayScene()
{
    ASSERT(!EditorPlayStopFlowController::IsPlaying());
    m_playing = true;

    ListLogger::GetInstance()->OnEditorPlay();
    BehaviourManager::RefreshAllBehaviourHoldersSynchronously();

    m_latestSceneBeforePlaying = SceneManager::GetActiveScene();

    Scene *sceneCopy = static_cast<Scene*>( m_latestSceneBeforePlaying->Clone() );
    if (sceneCopy)
    {
        SceneManager::SetActiveScene(sceneCopy);
    }

    EditorScene *edScene = static_cast<EditorScene*>(sceneCopy);
    edScene->SetFirstFoundCameraOrDefaultOne();

    EditorWindow *win = EditorWindow::GetInstance();
    win->tabContainerSceneGame->setCurrentWidget(win->tabGame);

    Screen::GetInstance()->setFocus();
}

void EditorPlayStopFlowController::StopScene()
{
    ASSERT(EditorPlayStopFlowController::IsPlaying());
    m_playing = false;

    Scene *sceneCopy = SceneManager::GetActiveScene();
    if (sceneCopy)
    {
        Hierarchy::GetInstance()->Clear();
        SceneManager::SetActiveScene(nullptr);
        delete sceneCopy;
    }

    SceneManager::SetActiveScene(m_latestSceneBeforePlaying);
    EditorScene *edScene = static_cast<EditorScene*>(m_latestSceneBeforePlaying);
    edScene->SetCamera( edScene->GetEditorCamera()->GetCamera() );

    EditorWindow *win = EditorWindow::GetInstance();
    win->tabContainerSceneGame->setCurrentWidget(win->tabScene);
}

