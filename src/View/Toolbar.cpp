#include "Toolbar.h"

#include "Debug.h"
#include "Scene.h"
#include "Camera.h"
#include "Application.h"
#include "EditorScene.h"
#include "EditorCamera.h"
#include "EditorWindow.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"
#include "ShortcutManager.h"

Toolbar *Toolbar::s_tb = nullptr;

Toolbar::Toolbar(QWidget *parent) : QWidget(parent) { }

Toolbar::~Toolbar()
{
}

void Toolbar::Init()
{
    EditorWindow *w = EditorWindow::GetInstance();
    Toolbar::s_tb = w->screenToolbar;
    Toolbar::s_tb->m_buttonTranslateMode = w->buttonTranslateMode;
    Toolbar::s_tb->m_buttonRotateMode    = w->buttonRotateMode;
    Toolbar::s_tb->m_buttonScaleMode     = w->buttonScaleMode;
    Toolbar::s_tb->m_buttonGlobalCoords  = w->buttonGlobalCoords;
    Toolbar::s_tb->m_buttonLocalCoords   = w->buttonLocalCoords;
    Toolbar::s_tb->m_buttonPlay          = w->buttonPlay;
    Toolbar::s_tb->m_buttonStop          = w->buttonStop;
    Toolbar::s_tb->m_button3D            = w->buttonOrthoPerspectiveMode;
    Toolbar::s_tb->m_gbufferAttachmentComboBox = w->comboBoxGBufferAttachment;

    connect(Toolbar::s_tb->m_buttonTranslateMode, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnTranslateClicked()));
    connect(Toolbar::s_tb->m_buttonRotateMode, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnRotateClicked()));
    connect(Toolbar::s_tb->m_buttonScaleMode, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnScaleClicked()));

    connect(Toolbar::s_tb->m_buttonGlobalCoords, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnGlobalCoordsClicked()));
    connect(Toolbar::s_tb->m_buttonLocalCoords, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnLocalCoordsClicked()));

    connect(Toolbar::s_tb->m_buttonPlay, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnPlayClicked()));
    connect(Toolbar::s_tb->m_buttonStop, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnStopClicked()));

    connect(Toolbar::s_tb->m_button3D, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnOrthoPerspectiveClicked()));

    connect(Toolbar::s_tb->m_gbufferAttachmentComboBox, SIGNAL(currentIndexChanged(int)),
            Toolbar::s_tb, SLOT(OnGBufferAttachmentIndexChanged(int)));

    Toolbar::s_tb->m_buttonTranslateMode->click();
    Toolbar::s_tb->m_buttonGlobalCoords->click();
    Toolbar::s_tb->m_buttonStop->click();

    Toolbar::s_tb->setFocusPolicy(Qt::ClickFocus);
}

void Toolbar::UnCheckTransformModeButtons()
{
    m_buttonTranslateMode->setChecked(false);
    m_buttonRotateMode->setChecked(false);
    m_buttonScaleMode->setChecked(false);
}

Toolbar *Toolbar::GetInstance()
{
    if (!s_tb)
    {
        Toolbar::Init();
    }

    return s_tb;
}

Toolbar::TransformMode Toolbar::GetSelectedTransformMode()
{
    return m_currentTransformMode;
}

bool Toolbar::IsPlaying()
{
    return m_playing;
}

bool Toolbar::IsInGlobalCoordsMode()
{
    return m_globalCoords;
}

void Toolbar::OnTranslateClicked()
{
    UnCheckTransformModeButtons();
    m_buttonTranslateMode->setChecked(true);
    m_currentTransformMode = TransformMode::Translate;
}

void Toolbar::OnRotateClicked()
{
    UnCheckTransformModeButtons();
    m_buttonRotateMode->setChecked(true);
    m_currentTransformMode = TransformMode::Rotate;
}

void Toolbar::OnScaleClicked()
{
    UnCheckTransformModeButtons();
    m_buttonScaleMode->setChecked(true);
    m_currentTransformMode = TransformMode::Scale;
}

void Toolbar::OnGlobalCoordsClicked()
{
    m_buttonGlobalCoords->setChecked(true);
    m_buttonLocalCoords->setChecked(false);
    m_globalCoords = true;
}

void Toolbar::OnLocalCoordsClicked()
{
    m_buttonGlobalCoords->setChecked(false);
    m_buttonLocalCoords->setChecked(true);
    m_globalCoords = false;
}

void Toolbar::OnPlayClicked()
{
    m_buttonPlay->setChecked(true);
    m_buttonStop->setChecked(false);

    if (m_playing == false)
    {
        m_playing = true;
        ListLogger::GetInstance()->OnEditorPlay();
        Application::GetInstance()->OnPlay();
    }

    Screen::GetInstance()->setFocus();
}

void Toolbar::OnStopClicked()
{
    m_buttonPlay->setChecked(false);
    m_buttonStop->setChecked(true);
    if (m_playing)
    {
        m_playing = false;
        Application::GetInstance()->OnStop();
    }
}

void Toolbar::OnOrthoPerspectiveClicked()
{
    Scene *currentScene = SceneManager::GetActiveScene();
    ASSERT(currentScene);

    Camera *cam = currentScene->GetCamera(); ASSERT(cam);
    EditorCamera *edCam = dynamic_cast<EditorCamera*>( cam->gameObject->parent );
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
    else if (newIndex == 1)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::Attachment::Position);
    }
    else if (newIndex == 2)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::Attachment::Normal);
    }
    else if (newIndex == 3)
    {
        gp->SetGBufferAttachmentToBeRendered(GBuffer::Attachment::Uv);
    }
    else if (newIndex == 4)
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
}

void Toolbar::OnShortcutPressed()
{
    if (ShortcutManager::IsPressed({Input::Key::Control, Input::Key::P}))
    {
        if (m_playing)
        {
            m_buttonStop->click();
        }
        else
        {
            m_buttonPlay->click();
        }
    }
}

