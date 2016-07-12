#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QAction>
#include <QToolButton>

#include "Input.h"
#include "IWindowEventManagerListener.h"

class Toolbar : public QWidget
                ,public IWindowEventManagerListener
{
    Q_OBJECT

public:
    enum TransformMode
    {
        Translate, Rotate, Scale
    };

private:
    static Toolbar *s_tb;
    bool m_globalCoords = false;
    bool m_playing = false;

    QTimer *p_keyTimer = nullptr;
    QToolButton *p_buttonTranslateMode = nullptr;
    QToolButton *p_buttonRotateMode    = nullptr;
    QToolButton *p_buttonScaleMode     = nullptr;
    QToolButton *p_buttonGlobalCoords  = nullptr;
    QToolButton *p_buttonLocalCoords   = nullptr;
    QToolButton *p_buttonPlay          = nullptr;
    QToolButton *p_buttonStop         = nullptr;

    TransformMode m_currentTransformMode = TransformMode::Translate;

    void UnCheckTransformModeButtons();

public:
    explicit Toolbar(QWidget *parent = nullptr);
    virtual ~Toolbar();

    static void Init();
    static Toolbar* GetInstance();

    TransformMode GetSelectedTransformMode();

    bool IsPlaying();
    bool IsInGlobalCoordsMode();

public slots:

    void OnTranslateClicked();
    void OnRotateClicked();
    void OnScaleClicked();
    void OnGlobalCoordsClicked();
    void OnLocalCoordsClicked();
    void OnPlayClicked();
    void OnStopClicked();
    void CheckKeyPressed();
};

#endif // TOOLBAR_H
