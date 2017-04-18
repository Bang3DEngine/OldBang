#include "Bang/Toolbar.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/EditorState.h"
#include "Bang/Application.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorCamera.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/EditorPlayFlow.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/BehaviourManager.h"

Toolbar *Toolbar::s_tb = nullptr;

Toolbar::Toolbar(QWidget *parent)
: QWidget(parent),
  m_playStopShortcut     (this, KSeq("Ctrl+P"), SLOT(OnPlayStopShortcut())),
  m_pauseShortcut        (this, KSeq("Ctrl+Shift+P"), SLOT(OnPauseShortcut())),
  m_translateShortcut    (this, KSeq("W"),      SLOT(OnTranslateClicked())),
  m_rotateShortcut       (this, KSeq("E"),      SLOT(OnRotateClicked())),
  m_scaleShortcut        (this, KSeq("R"),      SLOT(OnScaleClicked())),
  m_rectTransformShortcut(this, KSeq("T"),      SLOT(OnRectTransformClicked()))

{
}

Toolbar::~Toolbar()
{
}

void Toolbar::Init()
{
    EditorWindow *w = EditorWindow::GetInstance();
    Toolbar::s_tb = w->screenToolbar;
    Toolbar *tb = Toolbar::s_tb;
    tb->m_buttonTranslateMode       = w->buttonTranslateMode;
    tb->m_buttonRotateMode          = w->buttonRotateMode;
    tb->m_buttonScaleMode           = w->buttonScaleMode;
    tb->m_buttonRectTransformMode   = w->buttonRectTransformMode;
    tb->m_buttonGlobalCoords        = w->buttonGlobalCoords;
    tb->m_buttonLocalCoords         = w->buttonLocalCoords;
    tb->m_buttonShowGizmos          = w->buttonShowGizmos;
    tb->m_buttonFXAA                = w->buttonFXAA;
    tb->m_buttonPlay                = w->buttonPlay;
    tb->m_buttonPause               = w->buttonPause;
    tb->m_buttonStepFrame           = w->buttonStepFrame;
    tb->m_buttonStop                = w->buttonStop;
    tb->m_button3D                  = w->buttonOrthoPerspectiveMode;
    tb->m_gbufferAttachmentComboBox = w->comboBoxGBufferAttachment;

    connect(tb->m_buttonTranslateMode, SIGNAL(clicked()),
            tb, SLOT(OnTranslateClicked()));
    connect(tb->m_buttonRotateMode, SIGNAL(clicked()),
            tb, SLOT(OnRotateClicked()));
    connect(tb->m_buttonScaleMode, SIGNAL(clicked()),
            tb, SLOT(OnScaleClicked()));
    connect(tb->m_buttonRectTransformMode, SIGNAL(clicked()),
            tb, SLOT(OnRectTransformClicked()));

    connect(tb->m_buttonGlobalCoords, SIGNAL(clicked()),
            tb, SLOT(OnGlobalCoordsClicked()));
    connect(tb->m_buttonLocalCoords, SIGNAL(clicked()),
            tb, SLOT(OnLocalCoordsClicked()));

    connect(tb->m_buttonFXAA, SIGNAL(clicked(bool)), tb,
            SLOT(OnFXAAClicked(bool)));
    tb->OnFXAAClicked( tb->m_buttonFXAA->isChecked() );

    connect(tb->m_buttonShowGizmos, SIGNAL(clicked(bool)),
            tb, SLOT(OnShowGizmosClicked(bool)));
    tb->OnShowGizmosClicked( tb->m_buttonShowGizmos->isChecked() );

    connect(tb->m_buttonPlay,  SIGNAL(clicked()), tb, SLOT(OnPlayClicked()));
    connect(tb->m_buttonPause, SIGNAL(clicked()), tb, SLOT(OnPauseClicked()));
    connect(tb->m_buttonStepFrame, SIGNAL(clicked()),
            tb, SLOT(OnStepFrameClicked()));
    connect(tb->m_buttonStop,  SIGNAL(clicked()), tb, SLOT(OnStopClicked()));

    connect(tb->m_button3D, SIGNAL(clicked()),
            tb, SLOT(OnOrthoPerspectiveClicked()));

    connect(tb->m_gbufferAttachmentComboBox, SIGNAL(currentIndexChanged(int)),
            tb, SLOT(OnGBufferAttachmentIndexChanged(int)));

    tb->m_buttonTranslateMode->click();
    tb->m_buttonGlobalCoords->click();
    tb->m_buttonStop->click();

    tb->setFocusPolicy(Qt::ClickFocus);
}

void Toolbar::UnCheckTransformModeButtons()
{
    m_buttonTranslateMode->setChecked(false);
    m_buttonRotateMode->setChecked(false);
    m_buttonScaleMode->setChecked(false);
    m_buttonRectTransformMode->setChecked(false);
}

Toolbar *Toolbar::GetInstance()
{
    if (!s_tb) { Toolbar::Init(); }
    return s_tb;
}

void Toolbar::SetTransformMode(EditorState::TransformMode transformMode)
{
    UnCheckTransformModeButtons();
    if (transformMode == EditorState::TransformMode::Translate)
    {
        m_buttonTranslateMode->click();
    }
    else if (transformMode == EditorState::TransformMode::Rotate)
    {
        m_buttonRotateMode->click();
    }
    else if (transformMode == EditorState::TransformMode::Scale)
    {
        m_buttonScaleMode->click();
    }
    else if (transformMode == EditorState::TransformMode::RectTransform)
    {
        m_buttonRectTransformMode->click();
    }
}

void Toolbar::OnEditorUpdate()
{
    m_buttonPlay->setChecked( EditorState::IsPlaying() );
    m_buttonPause->setChecked( EditorState::IsPaused() );
    m_buttonStepFrame->setChecked( !EditorState::IsStopped() );
    m_buttonStop->setChecked( EditorState::IsStopped() );
}

void Toolbar::OnTranslateClicked()
{
    UnCheckTransformModeButtons();
    m_buttonTranslateMode->setChecked(true);
    EditorState::GetInstance()->m_currentTransformMode =
            EditorState::TransformMode::Translate;
}

void Toolbar::OnRotateClicked()
{
    UnCheckTransformModeButtons();
    m_buttonRotateMode->setChecked(true);
    EditorState::GetInstance()->m_currentTransformMode =
            EditorState::TransformMode::Rotate;
}

void Toolbar::OnScaleClicked()
{
    UnCheckTransformModeButtons();
    m_buttonScaleMode->setChecked(true);
    EditorState::GetInstance()->m_currentTransformMode =
            EditorState::TransformMode::Scale;
}

void Toolbar::OnRectTransformClicked()
{
    UnCheckTransformModeButtons();
    m_buttonRectTransformMode->setChecked(true);
    EditorState::GetInstance()->m_currentTransformMode =
            EditorState::TransformMode::RectTransform;
}

void Toolbar::OnGlobalCoordsClicked()
{
    m_buttonGlobalCoords->setChecked(true);
    m_buttonLocalCoords->setChecked(false);
    EditorState::GetInstance()->m_globalCoords = true;
}

void Toolbar::OnLocalCoordsClicked()
{
    m_buttonGlobalCoords->setChecked(false);
    m_buttonLocalCoords->setChecked(true);
    EditorState::GetInstance()->m_globalCoords = false;
}

void Toolbar::OnFXAAClicked(bool fxaa)
{
    GraphicPipeline::GetActive()->SetFXAA(fxaa);
}

void Toolbar::OnShowGizmosClicked(bool showGizmos)
{
    if (EditorState::IsShowingGameTab())
    {
        EditorState::GetInstance()->m_showGizmosGame  = showGizmos;
    }
    else
    {
        EditorState::GetInstance()->m_showGizmosScene = showGizmos;
    }
}

void Toolbar::OnPlayClicked()
{
    EditorPlayFlow::OnPlayClicked();
}

void Toolbar::OnPauseClicked()
{
    EditorPlayFlow::OnPauseClicked();
}

void Toolbar::OnStepFrameClicked()
{
    EditorPlayFlow::OnStepFrameClicked();
}

void Toolbar::OnStopClicked()
{
    EditorPlayFlow::OnStopClicked();
}

void Toolbar::OnOrthoPerspectiveClicked()
{
    Scene *currentScene = SceneManager::GetActiveScene();
    ENSURE(currentScene);

    Camera *cam = currentScene->GetCamera(); ENSURE(cam);
    EditorCamera *edCam = cam->Cast<EditorCamera>( cam->gameObject->parent );
    ENSURE(edCam);

    bool mode3D = m_button3D->isChecked();
    edCam->SwitchProjectionModeTo(mode3D);
}

void Toolbar::OnGBufferAttachmentIndexChanged(int newIndex)
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();

    if (newIndex == 0)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::AttColor);
    }
    else if (newIndex == 1)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::AttNormalDepth);
    }
    else if (newIndex == 2)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::AttDiffuse);
    }
}

void Toolbar::OnSceneGameTabChanged()
{
    if (EditorState::IsShowingGameTab())
    {
        m_buttonShowGizmos->setChecked(EditorState::ShowGizmosGameEnabled());

        m_buttonTranslateMode->setVisible(false);
        m_buttonRotateMode->setVisible(false);
        m_buttonScaleMode->setVisible(false);
        m_buttonRectTransformMode->setVisible(false);
        m_buttonGlobalCoords->setVisible(false);
        m_buttonLocalCoords->setVisible(false);
        m_button3D->setVisible(false);
    }
    else
    {
        m_buttonShowGizmos->setChecked(EditorState::ShowGizmosSceneEnabled());

        m_buttonTranslateMode->setVisible(true);
        m_buttonRotateMode->setVisible(true);
        m_buttonScaleMode->setVisible(true);
        m_buttonRectTransformMode->setVisible(true);
        m_buttonGlobalCoords->setVisible(true);
        m_buttonLocalCoords->setVisible(true);
        m_button3D->setVisible(true);
    }
}

bool Toolbar::OnPlayStopShortcut()
{
    if      (EditorState::IsPlaying()) { m_buttonStop->click(); }
    else if (EditorState::IsPaused())  { m_buttonPause->click(); }
    else if (EditorState::IsStopped()) { m_buttonPlay->click(); }
    return true;
}

void Toolbar::OnPauseShortcut()
{
    m_buttonPause->click();
}
