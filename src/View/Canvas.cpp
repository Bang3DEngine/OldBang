#include "Canvas.h"

#include "WindowMain.h"

int Canvas::width = 0;
int Canvas::height = 0;
int Canvas::RedrawDelay = 1;
float Canvas::aspectRatio = 1.0f;
Canvas *Canvas::stCanvas = nullptr;
unsigned long long Canvas::lastRenderTime = 0;

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    connect(&drawTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    drawTimer.setInterval(Canvas::RedrawDelay);
    drawTimer.start();

    windowMain = WindowMain::GetInstance();
    Input::Init();

    stCanvas = this;
}

void Canvas::initializeGL()
{
    windowMain->buttonPauseResume->setText( (paused ? QString("Resume") : QString("Pause")) );

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
        lastRenderTime = Time::GetNow();
        if(!paused)
        {
            currentStage->_OnUpdate();
        }

        currentStage->_OnRender();
    }

    QGLWidget::swapBuffers();

    Input::OnNewFrame();
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    width = w;
    height = h;
    aspectRatio = float(w) / h;

    if(currentStage != nullptr)
    {
        currentStage->_OnResize(w,h);
    }
}

Stage *Canvas::AddStage(const std::string &name)
{
    Stage *st = new Stage();
    st->name = name;
    stages.push_back(st);
    return st;
}

void Canvas::AddStage(Stage *stage)
{
    stages.push_back(stage);
}

void Canvas::SetCurrentStage(const std::string &name)
{
    if(currentStage != nullptr)
    {
        currentStage->_OnDestroy();
    }

    for(auto it = stages.begin(); it != stages.end(); ++it)
    {
        if((*it)->name == name)
        {
            if(currentStage != (*it))
            {
                currentStage = (*it);
                currentStage->_OnStart();

                WindowMain::GetInstance()->widgetHierarchy->FillDownwards(currentStage);
            }
            return;
        }
    }

    Logger_Warn("Could not change Stage to '" << name << "', "<<
                   "because no stage with this name is added to the Canvas.");
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

Canvas *Canvas::GetInstance()
{
    return stCanvas;
}

float Canvas::GetAspectRatio()
{
    return aspectRatio;
}

int Canvas::GetWidth()
{
    return width;
}

int Canvas::GetHeight()
{
    return height;
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    Input::HandleInputMousWheel(event);
    QGLWidget::wheelEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    Input::HandleInputMouseMove(event);
    QGLWidget::mouseMoveEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    Input::HandleInputMousePress(event);
    QGLWidget::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Input::HandleInputMouseRelease(event);
    QGLWidget::mouseReleaseEvent(event);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    Input::HandleInputKeyPress(event);
    QGLWidget::keyPressEvent(event);
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    Input::HandleInputKeyReleased(event);
    QGLWidget::keyReleaseEvent(event);
}

void Canvas::OnTopKekPressed()
{
    Prefab *p;
    if(rand()%2 == 0)
        p = AssetsManager::GetAsset<Prefab>("res/Assets/prefabTest.bprefab");
    else
        p = AssetsManager::GetAsset<Prefab>("res/Assets/luigiPrefab.bprefab");

    Entity *e = p->Instantiate();

    Entity *selected = WindowMain::GetInstance()->widgetHierarchy->GetFirstSelectedEntity();

    if(selected != nullptr) selected->AddChild(e);
    else currentStage->AddChild(e);
}

void Canvas::OnPauseResumeButtonPressed()
{
    paused = !paused;
    WindowMain::GetInstance()->buttonPauseResume->setText( (paused ? QString("Resume") : QString("Pause")) );
}
