#include "Toolbar.h"

#include "WindowMain.h"

Toolbar *Toolbar::tb = nullptr;

Toolbar::Toolbar(QWidget *parent) : QWidget(parent) { }

Toolbar::~Toolbar()
{
    delete keyTimer;
}

void Toolbar::Init()
{
    WindowMain *w = WindowMain::GetInstance();
    Toolbar::tb = w->widgetToolbar;
    Toolbar::tb->buttonTranslateMode = w->buttonTranslateMode;
    Toolbar::tb->buttonTranslateMode->setChecked(true);
    Toolbar::tb->buttonRotateMode    = w->buttonRotateMode;
    Toolbar::tb->buttonScaleMode     = w->buttonScaleMode;
    Toolbar::tb->buttonGlobalCoords  = w->buttonGlobalCoords;

    Toolbar::tb->keyTimer = new QTimer();
    Toolbar::tb->keyTimer->start(10);
    connect(Toolbar::tb->keyTimer, SIGNAL(timeout()),
            Toolbar::tb, SLOT(CheckKeyPressed()));

    connect(Toolbar::tb->buttonTranslateMode, SIGNAL(clicked()),
            Toolbar::tb, SLOT(OnTranslateClicked()));

    connect(Toolbar::tb->buttonRotateMode, SIGNAL(clicked()),
            Toolbar::tb, SLOT(OnRotateClicked()));

    connect(Toolbar::tb->buttonScaleMode, SIGNAL(clicked()),
            Toolbar::tb, SLOT(OnScaleClicked()));

    Toolbar::tb->setFocusPolicy(Qt::ClickFocus);
}

void Toolbar::UnCheckTransformModeButtons()
{
    buttonTranslateMode->setChecked(false);
    buttonRotateMode->setChecked(false);
    buttonScaleMode->setChecked(false);
}

Toolbar *Toolbar::GetInstance()
{
    if (tb == nullptr)
    {
        Toolbar::Init();
    }

    return tb;
}

Toolbar::TransformMode Toolbar::GetSelectedTransformMode()
{
    return currentTransformMode;
}

bool Toolbar::GetGlobalCoordsMode()
{
    return buttonGlobalCoords->isChecked();
}

void Toolbar::OnTranslateClicked()
{
    UnCheckTransformModeButtons();
    buttonTranslateMode->setChecked(true);
    currentTransformMode = TransformMode::Translate;
}

void Toolbar::OnRotateClicked()
{
    UnCheckTransformModeButtons();
    buttonRotateMode->setChecked(true);
    currentTransformMode = TransformMode::Rotate;
}

void Toolbar::OnScaleClicked()
{
    UnCheckTransformModeButtons();
    buttonScaleMode->setChecked(true);
    currentTransformMode = TransformMode::Scale;
}

void Toolbar::CheckKeyPressed()
{
    if(Input::GetKeyDown(Input::Key::W))
    {
        buttonTranslateMode->click();
    }
    else if(Input::GetKeyDown(Input::Key::E))
    {
        buttonRotateMode->click();
    }
    else if(Input::GetKeyDown(Input::Key::R))
    {
        buttonScaleMode->click();
    }
}

