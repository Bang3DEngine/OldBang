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
    static Toolbar *tb;

    QTimer *keyTimer = nullptr;
    QToolButton *buttonTranslateMode = nullptr;
    QToolButton *buttonRotateMode    = nullptr;
    QToolButton *buttonScaleMode     = nullptr;
    TransformMode currentTransformMode = TransformMode::Translate;

    QToolButton *buttonGlobalCoords  = nullptr;

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
