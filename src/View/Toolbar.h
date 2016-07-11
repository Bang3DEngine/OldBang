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

    QTimer *p_keyTimer = nullptr;
    QToolButton *p_buttonTranslateMode = nullptr;
    QToolButton *p_buttonRotateMode    = nullptr;
    QToolButton *p_buttonScaleMode     = nullptr;
    QToolButton *p_buttonGlobalCoords  = nullptr;

    TransformMode m_currentTransformMode = TransformMode::Translate;

    void UnCheckTransformModeButtons();

public:
    explicit Toolbar(QWidget *parent = nullptr);
    virtual ~Toolbar();

    static void Init();
    static Toolbar* GetInstance();

    TransformMode GetSelectedTransformMode();
    bool GetGlobalCoordsMode();

public slots:

    void OnTranslateClicked();
    void OnRotateClicked();
    void OnScaleClicked();
    void CheckKeyPressed();
};

#endif // TOOLBAR_H
