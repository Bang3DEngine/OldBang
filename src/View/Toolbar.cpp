#include "Toolbar.h"

#include "WindowMain.h"
#include "ShortcutManager.h"

Toolbar *Toolbar::s_tb = nullptr;

Toolbar::Toolbar(QWidget *parent) : QWidget(parent) { }

Toolbar::~Toolbar()
{
}

void Toolbar::Init()
{
    WindowMain *w = WindowMain::GetInstance();
    Toolbar::s_tb = w->widgetToolbar;
    Toolbar::s_tb->m_buttonTranslateMode = w->buttonTranslateMode;
    Toolbar::s_tb->m_buttonRotateMode    = w->buttonRotateMode;
    Toolbar::s_tb->m_buttonScaleMode     = w->buttonScaleMode;
    Toolbar::s_tb->m_buttonGlobalCoords  = w->buttonGlobalCoords;
    Toolbar::s_tb->m_buttonLocalCoords   = w->buttonLocalCoords;
    Toolbar::s_tb->m_buttonPlay          = w->buttonPlay;
    Toolbar::s_tb->m_buttonStop          = w->buttonStop;

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
    m_playing = true;

    Screen::GetInstance()->setFocus();
}

void Toolbar::OnStopClicked()
{
    m_buttonPlay->setChecked(false);
    m_buttonStop->setChecked(true);
    m_playing = false;
}

void Toolbar::OnShortcutsUpdate()
{
    if (ShortcutManager::IsPressed(Input::Key::W))
    {
        m_buttonTranslateMode->click();
    }
    else if (ShortcutManager::IsPressed(Input::Key::E))
    {
        m_buttonRotateMode->click();
    }
    else if (ShortcutManager::IsPressed(Input::Key::R))
    {
        m_buttonScaleMode->click();
    }
    else if (ShortcutManager::IsPressed({Input::Key::Control, Input::Key::P}) &&
             !m_playShortcutRecentlyUsed)
    {
        if (m_playing)
        {
            m_buttonStop->click();
        }
        else
        {
            m_buttonPlay->click();
        }

        m_playShortcutRecentlyUsed = true;
    }

    if (!ShortcutManager::IsPressed({Input::Key::Control, Input::Key::P}))
    {
        m_playShortcutRecentlyUsed = false;
    }
}

