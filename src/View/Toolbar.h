#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QAction>
#include <QComboBox>
#include <QToolButton>

#include "Input.h"
#include "IShortcutListener.h"
#include "IWindowEventManagerListener.h"

class Toolbar : public QWidget,
                public IWindowEventManagerListener,
                public IShortcutListener
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

    QToolButton *m_buttonTranslateMode = nullptr;
    QToolButton *m_buttonRotateMode    = nullptr;
    QToolButton *m_buttonScaleMode     = nullptr;
    QToolButton *m_buttonGlobalCoords  = nullptr;
    QToolButton *m_buttonLocalCoords   = nullptr;
    QToolButton *m_buttonPlay          = nullptr;
    QToolButton *m_buttonStop          = nullptr;
    QToolButton *m_button3D            = nullptr;
    QComboBox   *m_gbufferAttachmentComboBox = nullptr;

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
    void OnOrthoPerspectiveClicked();
    void OnGBufferAttachmentIndexChanged(int newIndex);

    void OnShortcutPressedKey(Input::Key key) override;
    void OnShortcutPressed() override;
};

#endif // TOOLBAR_H
