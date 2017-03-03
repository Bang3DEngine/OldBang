#include "Toolbar.h"

#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "EditorState.h"
#include "Application.h"
#include "EditorScene.h"
#include "EditorCamera.h"
#include "EditorWindow.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"
#include "ShortcutManager.h"
#include "BehaviourManager.h"
#include "EditorPlayStopFlowController.h"

Toolbar *Toolbar::s_tb = nullptr;

Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
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
    tb->m_buttonMSAA                = w->buttonMSAA;
    tb->m_buttonPlay                = w->buttonPlay;
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

    connect(tb->m_buttonMSAA, SIGNAL(clicked(bool)), tb,
            SLOT(OnMSAAClicked(bool)));
    tb->OnMSAAClicked( tb->m_buttonMSAA->isChecked() );

    connect(tb->m_buttonShowGizmos, SIGNAL(clicked(bool)),
            tb, SLOT(OnShowGizmosClicked(bool)));
    tb->OnShowGizmosClicked( tb->m_buttonShowGizmos->isChecked() );

    connect(tb->m_buttonPlay, SIGNAL(clicked()), tb, SLOT(OnPlayClicked()));
    connect(tb->m_buttonStop, SIGNAL(clicked()), tb, SLOT(OnStopClicked()));

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
    if (!s_tb)
    {
        Toolbar::Init();
    }
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

void Toolbar::OnMSAAClicked(bool msaa)
{
    GraphicPipeline::GetActive()->SetMSAA( msaa ? 2 : 1 );
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
    m_buttonPlay->setChecked(true);
    m_buttonStop->setChecked(false);

    bool startedPlaying = EditorPlayStopFlowController::OnPlayClicked();
    if (!startedPlaying)
    {
        m_buttonPlay->setChecked(false); // Revert the checking of the tool buttons
        m_buttonStop->setChecked(true);
    }
}

void Toolbar::OnStopClicked()
{
    m_buttonPlay->setChecked(false);
    m_buttonStop->setChecked(true);

    EditorPlayStopFlowController::OnStopClicked();
}

void Toolbar::OnOrthoPerspectiveClicked()
{
    Scene *currentScene = SceneManager::GetActiveScene();
    ASSERT(currentScene);

    Camera *cam = currentScene->GetCamera(); ASSERT(cam);
    EditorCamera *edCam = cam->Cast<EditorCamera>( cam->gameObject->parent );
    ASSERT(edCam);

    bool mode3D = m_button3D->isChecked();
    edCam->SwitchProjectionModeTo(mode3D);
}

void Toolbar::OnGBufferAttachmentIndexChanged(int newIndex)
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();

    if (newIndex == 0)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::Attachment::Color);
    }
    else if (newIndex == 2)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::Attachment::Normal);
    }
    else if (newIndex == 3)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::Attachment::Diffuse);
    }
}

void Toolbar::OnShortcutPressedKey(Input::Key key)
{
    if (key == Input::Key::W)
    {
        m_buttonTranslateMode->click();
    }
    else if (key == Input::Key::E)
    {
        m_buttonRotateMode->click();
    }
    else if (key == Input::Key::R)
    {
        m_buttonScaleMode->click();
    }
    else if (key == Input::Key::T)
    {
        m_buttonRectTransformMode->click();
    }
}

void Toolbar::OnShortcutPressed()
{
    if (ShortcutManager::IsPressed({Input::Key::Control, Input::Key::P}))
    {
        if (EditorState::IsPlaying())
        {
            m_buttonStop->click();
        }
        else
        {
            m_buttonPlay->click();
        }
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

