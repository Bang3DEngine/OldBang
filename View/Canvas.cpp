#include "Canvas.h"

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
}

void Canvas::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Canvas::paintGL()
{
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    Logger::Log("kasdjdsa");
    swapBuffers();
}
