#include "Canvas.h"

#include "SelectionFramebuffer.h"
#include "WindowMain.h"

int Canvas::width = 0;
int Canvas::height = 0;
int Canvas::RedrawDelay = 30;
float Canvas::aspectRatio = 1.0f;
Canvas *Canvas::stCanvas = nullptr;
unsigned long long Canvas::lastRenderTime = 0;

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    connect(&drawTimer, SIGNAL(timeout()), this, SLOT(update()));
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
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    Time::deltaTime = float(Time::GetNow() - lastRenderTime) / 1000.0f;
    if(currentScene != nullptr)
    {
        lastRenderTime = Time::GetNow();
        if(!paused)
        {
            currentScene->_OnUpdate();
        }

        //currentScene->_OnPreRender() is called from scene _OnRender
        currentScene->_OnRender();
    }

    Input::OnNewFrame();
}

void Canvas::updateGL()
{
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    width = w;
    height = h;
    aspectRatio = float(w) / h;

    if(currentScene != nullptr)
    {
        currentScene->_OnResize(w,h);
    }
}

Scene *Canvas::AddScene(const std::string &name)
{
    Scene *st = new Scene();
    st->name = name;
    scenes.push_back(st);
    return st;
}

void Canvas::AddScene(Scene *scene)
{
    scenes.push_back(scene);
}

void Canvas::SetCurrentScene(Scene *scene)
{
    if(currentScene != nullptr)
    {
        currentScene->_OnDestroy();
    }

    currentScene = scene;
    if(currentScene != nullptr)
    {
        currentScene->_OnStart();
        WindowMain::GetInstance()->widgetHierarchy->Refresh(currentScene);
    }
}

void Canvas::SetCurrentScene(const std::string &name)
{
    if(currentScene != nullptr)
    {
        currentScene->_OnDestroy();
    }

    for(auto it = scenes.begin(); it != scenes.end(); ++it)
    {
        if((*it)->name == name)
        {
            SetCurrentScene((*it));
            return;
        }
    }

    Logger_Warn("Could not change Scene to '" << name << "', "<<
                   "because no scene with this name is added to the Canvas.");
}

Scene *Canvas::GetCurrentScene() const
{
    return currentScene;
}

Scene *Canvas::GetScene(const std::string &name) const
{
    for(auto it = scenes.begin(); it != scenes.end(); ++it)
    {
        if((*it)->name == name) return (*it);
    }
    return nullptr;
}

void Canvas::RemoveScene(const std::string &name)
{
    for(auto it = scenes.begin(); it != scenes.end(); ++it)
    {
        if((*it)->name == name) { scenes.erase(it); return; }
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

void Canvas::SetCursor(Qt::CursorShape cs)
{
    WindowMain::GetInstance()->GetApplication()->
                setOverrideCursor( cs );
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
    Prefab *p = AssetsManager::GetAsset<Prefab>("res/Assets/luigiPrefab.bprefab");

    GameObject *e = p->Instantiate();

    GameObject *selected = WindowMain::GetInstance()->widgetHierarchy->GetFirstSelectedGameObject();

    if(selected != nullptr) selected->AddChild(e);
    else currentScene->AddChild(e);
}

void Canvas::OnPauseResumeButtonPressed()
{
    paused = !paused;
    WindowMain::GetInstance()->buttonPauseResume->setText( (paused ? QString("Resume") : QString("Pause")) );
}
