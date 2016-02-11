#include "Canvas.h"

int Canvas::RedrawDelay = 1;
float Canvas::aspectRatio = 1.0f;
unsigned long long Canvas::lastRenderTime = 0;

Canvas::Canvas(QWidget* parent) : QGLWidget(parent), currentStage(nullptr)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    connect(&drawTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    drawTimer.setInterval(Canvas::RedrawDelay);
    drawTimer.start();
}

void Canvas::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    lastRenderTime = Time::GetNow();
}

void Canvas::paintGL()
{
}

void Canvas::updateGL()
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    Time::deltaTime = float(Time::GetNow() - lastRenderTime) / 1000.0f;

    if(currentStage != nullptr)
    {
        currentStage->_OnUpdate();
        currentStage->_OnRender();
    }

    QGLWidget::swapBuffers();
    lastRenderTime = Time::GetNow();
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    aspectRatio = float(w) / h;
}

Stage *Canvas::AddStage(const std::string &name)
{
    Stage *st = new Stage();
    st->name = name;
    stages.push_back(st);
    return st;
}

void Canvas::SetCurrentStage(const std::string &name)
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

Stage *Canvas::GetCurrentStage() const
{
    return currentStage;
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

float Canvas::GetAspectRatio()
{
    return aspectRatio;
}
