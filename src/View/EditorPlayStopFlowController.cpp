#include "EditorPlayStopFlowController.h"

#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "ListLogger.h"
#include "EditorState.h"
#include "Application.h"
#include "EditorScene.h"
#include "EditorWindow.h"
#include "SceneManager.h"
#include "EditorCamera.h"
#include "BehaviourManager.h"

EditorPlayStopFlowController::EditorPlayStopFlowController()
{
}

bool EditorPlayStopFlowController::OnPlayClicked()
{
    return EditorPlayStopFlowController::GetInstance()->PlayScene();
}

void EditorPlayStopFlowController::OnStopClicked()
{
    EditorPlayStopFlowController::GetInstance()->StopScene();
}

EditorPlayStopFlowController *EditorPlayStopFlowController::GetInstance()
{
    return EditorWindow::GetInstance()->m_playStopController;
}

bool EditorPlayStopFlowController::PlayScene()
{
    ASSERT(!EditorState::IsPlaying(), "", return true);

    // Start refreshing all scene behaviours...
    BehaviourManager::RefreshAllBehaviours();

    // Wait for all behaviours to be loaded!
    while (!BehaviourManager::AllBehaviourHoldersUpdated())
    {
        if (BehaviourManager::SomeBehaviourWithError())
        {
            Debug_Error("Please fix all the behaviour errors before playing.");
            return false;
        }
        QThread::currentThread()->msleep(1000);


        // Let the timers tick. Needed to avoid a bug when a behaviour
        // changes while in this loop.
        // Thanks to the behaviour refreshing timer it's solved.
        Application::processEvents();
    }

    m_playing = true;
    ListLogger::GetInstance()->OnEditorPlay();

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

    return true;
}

void EditorPlayStopFlowController::StopScene()
{
    ASSERT(EditorState::IsPlaying());

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

