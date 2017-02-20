#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QAction>
#include <QComboBox>
#include <QToolButton>

#include "Input.h"
#include "EditorState.h"
#include "IShortcutListener.h"

class Toolbar : public QWidget,
                public IShortcutListener
{
    Q_OBJECT

public:

    explicit Toolbar(QWidget *parent = nullptr);
    virtual ~Toolbar();

    static void Init();
    static Toolbar* GetInstance();

    void SetTransformMode(EditorState::TransformMode transformMode);

public slots:

    void OnTranslateClicked();
    void OnRotateClicked();
    void OnScaleClicked();
    void OnRectTransformClicked();
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

    QToolButton *m_buttonTranslateMode = nullptr;
    QToolButton *m_buttonRotateMode    = nullptr;
    QToolButton *m_buttonScaleMode     = nullptr;
    QToolButton *m_buttonRectTransformMode = nullptr;
    QToolButton *m_buttonGlobalCoords  = nullptr;
    QToolButton *m_buttonLocalCoords   = nullptr;
    QToolButton *m_buttonShowGizmos    = nullptr;
    QToolButton *m_buttonPlay          = nullptr;
    QToolButton *m_buttonStop          = nullptr;
    QToolButton *m_button3D            = nullptr;
    QComboBox   *m_gbufferAttachmentComboBox = nullptr;

    void UnCheckTransformModeButtons();
};

#endif // TOOLBAR_H
