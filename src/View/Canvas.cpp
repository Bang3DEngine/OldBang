#include "Canvas.h"


#ifdef BANG_EDITOR
#include "WindowMain.h"
#include "SelectionFramebuffer.h"
#else
#include "GameWindow.h"
#endif

#ifdef BANG_EDITOR
WindowMain *Canvas::s_p_window = nullptr;
#else
GameWindow *Canvas::s_p_window = nullptr;
#endif

Canvas *Canvas::p_mainBinaryCanvas = nullptr;

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    m_clearColor = glm::vec4(1.0f) * 0.8f;

    connect(&m_drawTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_drawTimer.setInterval(Canvas::c_redrawDelay);
    m_drawTimer.start();

    #ifdef BANG_EDITOR
    Canvas::s_p_window = WindowMain::GetInstance();
    #else
    Canvas::s_p_window = GameWindow::GetInstance();
    #endif
}

void Canvas::InitFromMainBinary()
{
    Canvas::p_mainBinaryCanvas = Canvas::s_p_window->canvas;
}

void Canvas::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_lastRenderTime = Time::GetNow();
}

void Canvas::paintGL()
{
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.a);

    float deltaTime = float(Time::GetNow() - m_lastRenderTime) / 1000.0f;
    Time::GetInstance()->m_deltaTime = deltaTime;
    // Update mainBinary static deltaTime, for internal engine use
    // (this is not Behaviours' static deltaTime, theirs is updated in Behaviour _OnUpdate)
    Time::s_deltaTime = deltaTime;

    if(p_currentScene)
    {
        m_lastRenderTime = Time::GetNow();
        p_currentScene->_OnUpdate();
        p_currentScene->_OnRender(); //Note: _OnPreRender() is called from scene _OnRender
    }

    Input::GetInstance()->OnNewFrame();
}

void Canvas::updateGL()
{
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    m_width = w;
    m_height = h;
    m_aspectRatio = float(w) / h;

    if(p_currentScene )
    {
        p_currentScene->_OnResize(w,h);
    }
}

Scene *Canvas::AddScene(const std::string &name)
{
    Scene *st = new Scene();
    st->m_name = name;
    m_scenes.push_back(st);
    return st;
}

void Canvas::AddScene(Scene *scene)
{
    m_scenes.push_back(scene);
}

void Canvas::SetCurrentScene(Scene *scene)
{
    if(p_currentScene)
    {
        p_currentScene->_OnDestroy();
    }

    p_currentScene = scene;
    if(p_currentScene)
    {
        p_currentScene->_OnStart();
        #ifdef BANG_EDITOR
        Canvas::s_p_window->widgetHierarchy->Refresh();
        #endif
    }
}

void Canvas::SetCurrentScene(const std::string &name)
{
    if(p_currentScene )
    {
        p_currentScene->_OnDestroy();
    }

    for(auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
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

Scene *Canvas::GetCurrentScene()
{
    return Canvas::GetInstance()->p_currentScene;
}

Scene *Canvas::GetScene(const std::string &name) const
{
    for(auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        if((*it)->name == name) return (*it);
    }
    return nullptr;
}

void Canvas::RemoveScene(const std::string &name)
{
    for(auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        if((*it)->name == name) { m_scenes.erase(it); return; }
    }
}

Canvas *Canvas::GetInstance()
{
    return Canvas::p_mainBinaryCanvas;
}

float Canvas::GetAspectRatio()
{
    return Canvas::p_mainBinaryCanvas->m_aspectRatio;
}

int Canvas::GetWidth()
{
    return Canvas::p_mainBinaryCanvas->m_width;
}

int Canvas::GetHeight()
{
    return Canvas::p_mainBinaryCanvas->m_height;
}

void Canvas::SetCursor(Qt::CursorShape cs)
{
    Canvas::s_p_window->GetApplication()->setOverrideCursor( cs );
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    Input::GetInstance()->HandleInputMouseWheel(event);
    QGLWidget::wheelEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    Input::GetInstance()->HandleInputMouseMove(event);
    QGLWidget::mouseMoveEvent(event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    Input::GetInstance()->HandleInputMousePress(event);
    QGLWidget::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Input::GetInstance()->HandleInputMouseRelease(event);
    QGLWidget::mouseReleaseEvent(event);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    Input::GetInstance()->HandleInputKeyPress(event);
    QGLWidget::keyPressEvent(event);
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    Input::GetInstance()->HandleInputKeyReleased(event);
    QGLWidget::keyReleaseEvent(event);
}
