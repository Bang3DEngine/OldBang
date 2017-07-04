#include "Bang/EditorPlayFlow.h"

#include <QProgressDialog>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Console.h"
#include "Bang/G_Screen.h"
#include "Bang/EditorState.h"
#include "Bang/Application.h"
#include "Bang/EditorScene.h"
#include "Bang/AudioManager.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/EditorCamera.h"
#include "Bang/BehaviourManager.h"
#include "Bang/AnonymousAudioPlayer.h"

EditorPlayFlow::EditorPlayFlow()
{
}

bool EditorPlayFlow::OnPlayClicked()
{
    if ( EditorState::IsPlaying() ) { return true; }
    if ( EditorState::IsPaused()  ) { EditorPlayFlow::OnPauseClicked(); }
    return EditorPlayFlow::GetInstance()->PlayScene();
}

void EditorPlayFlow::OnPauseClicked()
{
    ENSURE( !EditorState::IsStopped() );
    EditorState::PlayState newPlayState = EditorState::IsPaused() ?
                                             EditorState::PlayState::Playing :
                                             EditorState::PlayState::Paused;

    if (newPlayState == EditorState::PlayState::Playing)
    {
        AudioManager::ResumeAllSounds();
    }
    else
    {
        AudioManager::PauseAllSounds();
    }

    EditorState::SetPlayingState(newPlayState);
}

void EditorPlayFlow::OnStepFrameClicked()
{
    ENSURE( !EditorState::IsStopped() );
    EditorPlayFlow::GetInstance()->m_stepNextFrame = true;

    EditorPlayFlow::OnPauseClicked(); // Toggle pause
}

void EditorPlayFlow::OnStopClicked()
{
    ENSURE( !EditorState::IsStopped() );
    EditorPlayFlow::GetInstance()->StopScene();
    EditorPlayFlow::GetInstance()->m_stepNextFrame = false;
}

void EditorPlayFlow::OnFrameFinished()
{
    EditorPlayFlow *e = EditorPlayFlow::GetInstance();
    if (EditorState::IsPlaying() && e->m_stepNextFrame)
    {
        EditorPlayFlow::OnPauseClicked();
        e->m_stepNextFrame = false;
    }
}

EditorPlayFlow *EditorPlayFlow::GetInstance()
{
    return EditorWindow::GetInstance()->m_playStopController;
}

bool EditorPlayFlow::PlayScene()
{
	if (EditorState::IsPlaying()) { return true; }

    AudioManager::StopAllSounds();
    if (!WaitForAllBehavioursToBeLoaded()) { return false; }

    EditorState::SetPlayingState(EditorState::PlayState::Playing);

    Hierarchy::GetInstance()->clearSelection();
    Console::GetInstance()->OnEditorPlay();
    Inspector::GetInstance()->Clear();

    // Pick the reference of the current scene, to restore it later
    Scene *sceneCopy = nullptr;
    p_latestSceneBeforePlaying = SceneManager::GetActiveScene();
    if (p_latestSceneBeforePlaying)
    {
        m_latestSceneBeforePlayingFilepath = SceneManager::GetActiveSceneFilepath();
        sceneCopy = p_latestSceneBeforePlaying->Clone();
        if (sceneCopy)
        {
            SceneManager::SetActiveScene(sceneCopy);
        }
    }

    if (sceneCopy)
    {
        EditorScene *edScene = Object::SCast<EditorScene>(sceneCopy);
        edScene->SetFirstFoundCameraOrDefaultOne();
    }

    EditorWindow *win = EditorWindow::GetInstance();
    win->tabContainerSceneGame->setCurrentWidget(win->tabGame);

    return true;
}

void EditorPlayFlow::StopScene()
{
    ENSURE(!EditorState::IsStopped());

    AudioManager::StopAllSounds();
    EditorState::SetPlayingState(EditorState::PlayState::Stopped);

    Scene *sceneCopy = SceneManager::GetActiveScene();
    if (sceneCopy)
    {
        Hierarchy::GetInstance()->Clear();
        Inspector::GetInstance()->Clear();
        SceneManager::SetActiveScene(nullptr);
        AnonymousAudioPlayer::StopAndClearEverything();
        delete sceneCopy;
    }

    SceneManager::SetActiveScene(p_latestSceneBeforePlaying);
    SceneManager::SetActiveSceneFilepath(m_latestSceneBeforePlayingFilepath);

    EditorWindow *win = EditorWindow::GetInstance();
    win->tabContainerSceneGame->setCurrentWidget(win->tabScene);
}

bool EditorPlayFlow::WaitForAllBehavioursToBeLoaded()
{
    EditorWindow *win = EditorWindow::GetInstance();
    QMainWindow *mainWin = win->GetMainWindow();

    // Create a progress dialog to show progress
    QProgressDialog progressDialog(mainWin);
    progressDialog.setRange(0, 100);
    progressDialog.setFixedSize(350, 100);
    progressDialog.setSizeGripEnabled(false);
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

    m_playingCanceled = false;
    bool success = BehaviourManager::PrepareBehavioursLibrary(
                                            false,
                                            Paths::ProjectLibrariesDir(),
                                            &m_playingCanceled);
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

void EditorPlayFlow::OnWaitingForBehavioursCanceled()
{
    m_playingCanceled = true;
}

