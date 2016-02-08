#include "Canvas.h"

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Canvas::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Canvas::paintGL()
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    if(currentStage != nullptr)
    {
        currentStage->_OnUpdate();
        currentStage->_OnDrawing();
    }

    QGLWidget::swapBuffers();
}

Stage *Canvas::AddStage(const std::string &name)
{
    Stage *st = new Stage();
    st->name = name;
    stages.push_back(st);
    return st;
}

void Canvas::SetStage(const std::string &name)
{
    for(auto it = stages.begin(); it != stages.end(); ++it)
    {
        if((*it)->name == name)
        {
            if(currentStage != (*it))
            {
                currentStage = (*it);
                currentStage->_OnStart();
            }
            return;
        }
    }
}

Stage *Canvas::GetStage(const std::string &name) const
{
    for(auto it = stages.begin(); it != stages.end(); ++it)
    {
        if((*it)->name == name) return (*it);
    }
    return nullptr;
}

void Canvas::RemoveStage(const std::string &name)
{
    for(auto it = stages.begin(); it != stages.end(); ++it)
    {
        if((*it)->name == name) { stages.erase(it); return; }
    }
}
