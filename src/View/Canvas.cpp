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

    for(DrawRequest dr : drawRequests)
    {
        dr.renderer->Render(dr.transform, MeshRenderer::DrawingMode::Triangles);
    }
    //drawRequests.clear();

    QGLWidget::swapBuffers();
}

Stage *Canvas::AddStage(const std::string &name)
{
    Stage *st = new Stage();
    st->name = name;
    stages.push_back(st);
    return st;
}

Stage *Canvas::GetStage(const std::string &name)
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

void Canvas::Draw(const MeshRenderer *renderer, const Transform *t) const
{
    drawRequests.push_back(DrawRequest(renderer, t));
}
