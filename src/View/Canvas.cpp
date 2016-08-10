#include "Canvas.h"

#ifdef BANG_EDITOR
#include "WindowMain.h"
#include "SelectionFramebuffer.h"
#else
#include "GameWindow.h"
#endif

#include "SingletonManager.h"

#ifdef BANG_EDITOR
WindowMain *Canvas::s_m_window = nullptr;
#else
GameWindow *Canvas::s_m_window = nullptr;
#endif

Canvas *Canvas::m_mainBinaryCanvas = nullptr;

Canvas::Canvas(QWidget* parent) : QGLWidget(parent)
{
    /*
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
    */

    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    connect(&m_drawTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_drawTimer.setInterval(Canvas::c_redrawDelay);
    m_drawTimer.start();

    #ifdef BANG_EDITOR
    Canvas::s_m_window = WindowMain::GetInstance();
    #else
    Canvas::s_m_window = GameWindow::GetInstance();
    #endif
}

void Canvas::InitFromMainBinary()
{
    #ifdef BANG_EDITOR
    Canvas::m_mainBinaryCanvas = static_cast<WindowMain*>(SingletonManager::GetInstance()->GetWindowSingleton())->canvas;
    #else
    Canvas::m_mainBinaryCanvas = static_cast<GameWindow*>(SingletonManager::GetInstance()->GetWindowSingleton())->canvas;
    #endif
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
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

    float deltaTime = float(Time::GetNow() - m_lastRenderTime) / 1000.0f;
    Time::GetInstance()->m_deltaTime = deltaTime;
    // Update mainBinary static deltaTime, for internal engine use
    // (this is not Behaviours' static deltaTime, theirs is updated in Behaviour _OnUpdate)
    Time::s_deltaTime = deltaTime;

    if (m_currentScene)
    {
        m_lastRenderTime = Time::GetNow();
        m_currentScene->_OnUpdate();
        m_currentScene->_OnRender(); //Note: _OnPreRender() is called from scene _OnRender
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

    if (m_currentScene )
    {
        m_currentScene->_OnResize(w,h);
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
    if (m_currentScene)
    {
        m_currentScene->_OnDestroy();
    }

    m_currentScene = scene;
    if (m_currentScene)
    {
        m_currentScene->_OnStart();
        #ifdef BANG_EDITOR
        Canvas::s_m_window->widgetHierarchy->Refresh();
        #endif
    }
}

void Canvas::SetCurrentScene(const std::string &name)
{
    if (m_currentScene )
    {
        m_currentScene->_OnDestroy();
    }

    for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        if ((*it)->name == name)
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
    Canvas *c = Canvas::GetInstance();
    return c ? c->m_currentScene : nullptr;
}

Scene *Canvas::GetScene(const std::string &name) const
{
    for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        if ((*it)->name == name) return (*it);
    }
    return nullptr;
}

void Canvas::RemoveScene(const std::string &name)
{
    for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
    {
        if ((*it)->name == name) { m_scenes.erase(it); return; }
    }
}

Canvas *Canvas::GetInstance()
{
    #ifdef BANG_EDITOR
    return static_cast<WindowMain*>(SingletonManager::GetInstance()->GetWindowSingleton())->canvas;
    #else
    return static_cast<GameWindow*>(SingletonManager::GetInstance()->GetWindowSingleton())->canvas;
    #endif
}

float Canvas::GetAspectRatio()
{
    return Canvas::m_mainBinaryCanvas->m_aspectRatio;
}

int Canvas::GetWidth()
{
    return Canvas::m_mainBinaryCanvas->m_width;
}

int Canvas::GetHeight()
{
    return Canvas::m_mainBinaryCanvas->m_height;
}

void Canvas::SetCursor(Qt::CursorShape cs)
{
    Canvas::s_m_window->GetApplication()->setOverrideCursor( cs );
}

#ifdef BANG_EDITOR
void Canvas::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
}

void Canvas::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();
}

void Canvas::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}

void Canvas::dropEvent(QDropEvent *e)
{
    Explorer *explorer = Explorer::GetInstance();
    Logger_Log("Drop!!!");
    if (e->source() == explorer)
    {
        Logger_Log("Drop from explorer!");
    }

    e->ignore();
}
#endif

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
