#include "Toolbar.h"

#include "WindowMain.h"

Toolbar *Toolbar::s_tb = nullptr;

Toolbar::Toolbar(QWidget *parent) : QWidget(parent) { }

Toolbar::~Toolbar()
{
    delete p_keyTimer;
}

void Toolbar::Init()
{
    WindowMain *w = WindowMain::GetInstance();
    Toolbar::s_tb = w->widgetToolbar;
    Toolbar::s_tb->p_buttonTranslateMode = w->buttonTranslateMode;
    Toolbar::s_tb->p_buttonRotateMode    = w->buttonRotateMode;
    Toolbar::s_tb->p_buttonScaleMode     = w->buttonScaleMode;
    Toolbar::s_tb->p_buttonGlobalCoords  = w->buttonGlobalCoords;
    Toolbar::s_tb->p_buttonLocalCoords   = w->buttonLocalCoords;
    Toolbar::s_tb->p_buttonPlay          = w->buttonPlay;
    Toolbar::s_tb->p_buttonStop          = w->buttonStop;

    Toolbar::s_tb->p_keyTimer = new QTimer();
    Toolbar::s_tb->p_keyTimer->start(1);

    connect(Toolbar::s_tb->p_keyTimer, SIGNAL(timeout()),
            Toolbar::s_tb, SLOT(CheckKeyPressed()));

    connect(Toolbar::s_tb->p_buttonTranslateMode, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnTranslateClicked()));
    connect(Toolbar::s_tb->p_buttonRotateMode, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnRotateClicked()));
    connect(Toolbar::s_tb->p_buttonScaleMode, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnScaleClicked()));

    connect(Toolbar::s_tb->p_buttonGlobalCoords, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnGlobalCoordsClicked()));
    connect(Toolbar::s_tb->p_buttonLocalCoords, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnLocalCoordsClicked()));

    connect(Toolbar::s_tb->p_buttonPlay, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnPlayClicked()));
    connect(Toolbar::s_tb->p_buttonStop, SIGNAL(clicked()),
            Toolbar::s_tb, SLOT(OnStopClicked()));

    Toolbar::s_tb->p_buttonTranslateMode->click();
    Toolbar::s_tb->p_buttonGlobalCoords->click();
    Toolbar::s_tb->p_buttonStop->click();

    Toolbar::s_tb->setFocusPolicy(Qt::ClickFocus);
}

void Toolbar::UnCheckTransformModeButtons()
{
    p_buttonTranslateMode->setChecked(false);
    p_buttonRotateMode->setChecked(false);
    p_buttonScaleMode->setChecked(false);
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
    p_buttonTranslateMode->setChecked(true);
    m_currentTransformMode = TransformMode::Translate;
}

void Toolbar::OnRotateClicked()
{
    UnCheckTransformModeButtons();
    p_buttonRotateMode->setChecked(true);
    m_currentTransformMode = TransformMode::Rotate;
}

void Toolbar::OnScaleClicked()
{
    UnCheckTransformModeButtons();
    p_buttonScaleMode->setChecked(true);
    m_currentTransformMode = TransformMode::Scale;
}

void Toolbar::OnGlobalCoordsClicked()
{
    p_buttonGlobalCoords->setChecked(true);
    p_buttonLocalCoords->setChecked(false);
    m_globalCoords = true;
}

void Toolbar::OnLocalCoordsClicked()
{
    p_buttonGlobalCoords->setChecked(false);
    p_buttonLocalCoords->setChecked(true);
    m_globalCoords = false;
}

void Toolbar::OnPlayClicked()
{
    p_buttonPlay->setChecked(true);
    p_buttonStop->setChecked(false);
    m_playing = true;
}

void Toolbar::OnStopClicked()
{
    p_buttonPlay->setChecked(false);
    p_buttonStop->setChecked(true);
    m_playing = false;
}

void Toolbar::CheckKeyPressed()
{
    if (Input::GetKeyDown(Input::Key::W))
    {
        p_buttonTranslateMode->click();
    }
    else if (Input::GetKeyDown(Input::Key::E))
    {
        p_buttonRotateMode->click();
    }
    else if (Input::GetKeyDown(Input::Key::R))
    {
        p_buttonScaleMode->click();
    }
    else if (Input::GetKeyDown(Input::Key::P) && Input::GetKey(Input::Key::Control))
    {
        if (m_playing)
        {
            p_buttonPlay->click();
        }
        else
        {
            p_buttonStop->click();
        }
    }
}

