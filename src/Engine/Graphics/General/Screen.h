#ifndef SCREEN_H
#define SCREEN_H

#include <GL/glew.h>

#include <QTimer>
#include <QThread>
#include <QGLWidget>
#include <QApplication>
#include "Bang/WinUndef.h"

#include "Bang/G_Screen.h"

class GameObject;
class GraphicPipeline;
class Screen: public QGLWidget,
              public G_Screen

{
    Q_OBJECT

public:
    Screen(QWidget *parent = nullptr);
    virtual ~Screen();

    void Initialize();
    void initializeGL() override;
    void paintGL() override;
    virtual void Render() const;
    void resizeGL(int w, int h) override;

    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();
    static Vector2 GetSize();
    static Vector2 GetPixelClipSize();

    static Screen *GetInstance();
    GraphicPipeline *GetGraphicPipeline() const;

protected:
    GraphicPipeline *m_gPipeline = nullptr;
};

#endif // SCREEN_H
