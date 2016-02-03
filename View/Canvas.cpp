#include "Canvas.h"

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
}

void Canvas::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.0, 1.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    Shader s(Shader::Type::Fragment, "Domain/main.cpp");
    std::cout << s.ToString() << std::endl;
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Canvas::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
