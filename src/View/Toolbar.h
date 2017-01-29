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

    explicit Toolbar(QWidget *parent = nullptr);
    virtual ~Toolbar();

    static void Init();
    static Toolbar* GetInstance();

    TransformMode GetSelectedTransformMode();

    bool IsPlaying() const;
    bool ShowGizmosEnabled() const;
    bool IsInGlobalCoordsMode() const;

public slots:

    void OnTranslateClicked();
    void OnRotateClicked();
    void OnScaleClicked();
    void OnGlobalCoordsClicked();
    void OnLocalCoordsClicked();
    void OnShowGizmosClicked(bool showGizmos);
    void OnPlayClicked();
    void OnStopClicked();
    void OnOrthoPerspectiveClicked();
    void OnGBufferAttachmentIndexChanged(int newIndex);

    void OnShortcutPressedKey(Input::Key key) override;
    void OnShortcutPressed() override;

    void OnSceneGameTabChanged();

private:
    static Toolbar *s_tb;

    bool m_globalCoords = false;
    bool m_playing = false;
    bool m_showGizmosScene = true;
    bool m_showGizmosGame = false;

    QToolButton *m_buttonTranslateMode = nullptr;
    QToolButton *m_buttonRotateMode    = nullptr;
    QToolButton *m_buttonScaleMode     = nullptr;
    QToolButton *m_buttonGlobalCoords  = nullptr;
    QToolButton *m_buttonLocalCoords   = nullptr;
    QToolButton *m_buttonShowGizmos    = nullptr;
    QToolButton *m_buttonPlay          = nullptr;
    QToolButton *m_buttonStop          = nullptr;
    QToolButton *m_button3D            = nullptr;
    QComboBox   *m_gbufferAttachmentComboBox = nullptr;

    TransformMode m_currentTransformMode = TransformMode::Translate;

    void UnCheckTransformModeButtons();
};

#endif // TOOLBAR_H
