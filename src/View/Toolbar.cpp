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
    Toolbar::s_tb->p_buttonTranslateMode->setChecked(true);
    Toolbar::s_tb->p_buttonRotateMode    = w->buttonRotateMode;
    Toolbar::s_tb->p_buttonScaleMode     = w->buttonScaleMode;
    Toolbar::s_tb->p_buttonGlobalCoords  = w->buttonGlobalCoords;

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

bool Toolbar::GetGlobalCoordsMode()
{
    return p_buttonGlobalCoords->isChecked();
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

void Toolbar::CheckKeyPressed()
{
    if(Input::GetKeyDown(Input::Key::W))
    {
        p_buttonTranslateMode->click();
    }
    else if(Input::GetKeyDown(Input::Key::E))
    {
        p_buttonRotateMode->click();
    }
    else if(Input::GetKeyDown(Input::Key::R))
    {
        p_buttonScaleMode->click();
    }
}

