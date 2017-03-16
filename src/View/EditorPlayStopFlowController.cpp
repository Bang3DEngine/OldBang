#include "EditorPlayStopFlowController.h"

#include <QProgressDialog>

#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Dialog.h"
#include "Console.h"
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

    if (!WaitForAllBehavioursToBeLoaded()) { return false; }

    m_playing = true;
    Hierarchy::GetInstance()->clearSelection();
    Console::GetInstance()->OnEditorPlay();
    Inspector::GetInstance()->OnEditorPlay();

    // Pick the reference of the current scene, to restore it later
    p_latestSceneBeforePlaying = SceneManager::GetActiveScene();

    Scene *sceneCopy =
            Object::SCast<Scene>( p_latestSceneBeforePlaying->Clone() );
    if (sceneCopy)
    {
        SceneManager::SetActiveScene(sceneCopy);
    }

    EditorScene *edScene = Object::SCast<EditorScene>(sceneCopy);
    edScene->SetFirstFoundCameraOrDefaultOne();

    EditorWindow *win = EditorWindow::GetInstance();
    win->tabContainerSceneGame->setCurrentWidget(win->tabGame);

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

    SceneManager::SetActiveScene(p_latestSceneBeforePlaying);
    EditorScene *edScene = Object::SCast<EditorScene>(
                p_latestSceneBeforePlaying);
    edScene->SetCamera( edScene->GetEditorCamera()->GetCamera() );

    EditorWindow *win = EditorWindow::GetInstance();
    win->tabContainerSceneGame->setCurrentWidget(win->tabScene);
}

bool EditorPlayStopFlowController::WaitForAllBehavioursToBeLoaded()
{
    EditorWindow *win = EditorWindow::GetInstance();
    QMainWindow *mainWin = win->GetMainWindow();

    m_playingCanceled = false;

    // Create a progress dialog to show progress
    QProgressDialog progressDialog(mainWin);
    progressDialog.setRange(0, 100);
    progressDialog.setModal(true); // Important to avoid Ctrl+P smashing
    progressDialog.setWindowTitle("Compiling behaviours");
    progressDialog.setLabelText(
                "Waiting for all behaviours to be correctly compiled...");
    progressDialog.show();

    BehaviourManager *bm = BehaviourManager::GetInstance();

    connect(&progressDialog, SIGNAL(canceled()),
            this, SLOT(OnWaitingForBehavioursCanceled()));
    connect(bm, SIGNAL(NotifyPrepareBehavioursLibraryProgressed(int)),
            &progressDialog, SLOT(setValue(int)));

    BehaviourManager::SetCurrentLibsDir( IO::GetProjectLibsRootAbs() );
    bool success = BehaviourManager::PrepareBehavioursLibrary(
                                            false, &m_playingCanceled);

    if (!success && !m_playingCanceled)
    {
        String errMsg = "Please fix all the behaviour errors before playing.";
        Dialog::Error("Error", errMsg + "\nCheck the Console.");
        Debug_Error(errMsg);
        return false;
    }

    disconnect(&progressDialog, SIGNAL(canceled()),
            this, SLOT(OnWaitingForBehavioursCanceled()));
    disconnect(bm, SIGNAL(NotifyPrepareBehavioursLibraryProgressed(int)),
               &progressDialog, SLOT(setValue(int)));

    progressDialog.close();

    return success && !m_playingCanceled;
}

void EditorPlayStopFlowController::OnWaitingForBehavioursCanceled()
{
    m_playingCanceled = true;
}

