#include "Screen.h"

#ifdef BANG_EDITOR
#include "Explorer.h"
#include "Hierarchy.h"
#include "EditorWindow.h"
#include "EditorScene.h"
#include "SelectionFramebuffer.h"
#else
#include "GameWindow.h"
#endif

#include "Application.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"
#include "ShortcutManager.h"
#include "SingletonManager.h"

#ifdef BANG_EDITOR
EditorWindow *Screen::s_m_window = nullptr;
#else
GameWindow *Screen::s_m_window = nullptr;
#endif

Screen *Screen::m_mainBinaryScreen = nullptr;

Screen::Screen(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    #ifdef BANG_EDITOR
    Screen::s_m_window = EditorWindow::GetInstance();
    #else
    Screen::s_m_window = GameWindow::GetInstance();
    #endif
}

void Screen::InitFromMainBinary()
{
    #ifdef BANG_EDITOR
    Screen::m_mainBinaryScreen = static_cast<EditorWindow*>(SingletonManager::GetInstance()->GetWindowSingleton())->screen;
    #else
    Screen::m_mainBinaryScreen = static_cast<GameWindow*>(SingletonManager::GetInstance()->GetWindowSingleton())->screen;
    #endif
}

void Screen::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Screen::paintGL()
{
    Render();
}

void Screen::Render()
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        GraphicPipeline::GetActive()->RenderScene(activeScene);
    }
    else
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Screen::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    m_width = w;
    m_height = h;
    m_aspectRatio = float(w) / h;

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        activeScene->_OnResize(w,h);
    }
}


Screen *Screen::GetInstance()
{
    #ifdef BANG_EDITOR
    return static_cast<EditorWindow*>(SingletonManager::GetInstance()->GetWindowSingleton())->screen;
    #else
    return static_cast<GameWindow*>(SingletonManager::GetInstance()->GetWindowSingleton())->screen;
    #endif
}

float Screen::GetAspectRatio()
{
    return Screen::m_mainBinaryScreen->m_aspectRatio;
}

int Screen::GetWidth()
{
    return Screen::m_mainBinaryScreen->m_width;
}

Vector2 Screen::GetSize()
{
    return Vector2(Screen::GetWidth(), Screen::GetHeight());
}

int Screen::GetHeight()
{
    return Screen::m_mainBinaryScreen->m_height;
}

void Screen::SetCursor(Qt::CursorShape cs)
{
    Application::GetInstance()->setOverrideCursor( cs );
}

#ifdef BANG_EDITOR
void Screen::OnShortcutPressed()
{
    if (hasFocus())
    {
        if ( ShortcutManager::IsPressed({Input::Key::Control, Input::Key::C}) )
        { // Copy
            Hierarchy::GetInstance()->m_hContextMenu.OnCopyClicked();
        }
        else if ( ShortcutManager::IsPressed({Input::Key::Control, Input::Key::V}) )
        { // Paste
            Hierarchy::GetInstance()->m_hContextMenu.OnPasteClicked();
        }
        else if ( ShortcutManager::IsPressed({Input::Key::Control, Input::Key::D}) )
        { // Duplicate
            Hierarchy::GetInstance()->m_hContextMenu.OnDuplicateClicked();
        }
        else if (ShortcutManager::IsPressed(Input::Key::Delete))
        { // Delete
            Hierarchy::GetInstance()->m_hContextMenu.OnDeleteClicked();
        }
    }
}

void Screen::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
}

void Screen::HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin)
{
    Scene *activeScene = SceneManager::GetActiveScene();
    EditorScene *scene = static_cast<EditorScene*>(activeScene);
    SelectionFramebuffer *sfb = scene->GetSelectionFramebuffer();
    int x = e->pos().x();
    int y = Screen::GetHeight() - e->pos().y();
    GameObject *overedGo = sfb->GetGameObjectInPosition(x, y);
    m_dragOrigin = origin;

    if (!m_gameObjectBeingDragged)
    {
        Explorer *explorer = Explorer::GetInstance();
        Hierarchy *hierarchy = Hierarchy::GetInstance();
        if (origin == hierarchy)
        {
            GameObject *selectedGo = hierarchy->GetFirstSelectedGameObject();
            if (selectedGo)
            {
                m_gameObjectBeingDragged = static_cast<GameObject*>(selectedGo->Clone());
            }
        }
        else if (origin == explorer)
        {
            File f = explorer->GetSelectedFile();
            if (f.IsPrefabAsset())
            {
                Prefab *prefab = new Prefab();
                prefab->ReadXMLInfoFromString(f.GetContents());
                m_gameObjectBeingDragged = prefab->InstantiateWithoutStarting();
                delete prefab;
            }
        }
    }
    else
    {
        Vector3 worldPos;
        if (overedGo)
        {
            worldPos = sfb->GetWorldPositionAt(x, y);
        }
        else
        {
            Camera *cam = scene->GetCamera();
            Vector2 ndcPos = Vector2(x, Screen::GetHeight() - y);
            ndcPos /= Screen::GetSize();
            ndcPos = ndcPos * 2.0f - 1.0f;
            ndcPos.y *= -1.0f;
            float z = Vector3::Distance(cam->transform->GetPosition(),
                                        scene->GetBoundingBox().GetCenter());
            worldPos = cam->ScreenNDCPointToWorld(ndcPos, z);
        }

        m_gameObjectBeingDragged->transform->SetPosition(worldPos);
    }

    if (m_gameObjectBeingDragged)
    {
        m_gameObjectBeingDragged->m_isDragged = true;
        if (m_gameObjectBeingDragged->parent != scene)
        {
            m_gameObjectBeingDragged->SetParent(scene);
        }
    }
}

void Screen::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();

    Scene *activeScene = SceneManager::GetActiveScene();
    NONULL(activeScene);

    EditorScene *scene = static_cast<EditorScene*>(activeScene);
    SelectionFramebuffer *sfb = scene->GetSelectionFramebuffer();
    int x = e->pos().x(), y = Screen::GetHeight() - e->pos().y();
    GameObject *overedGo = sfb->GetGameObjectInPosition(x, y);

    Explorer *explorer = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (e->source() == explorer)
    {
        File f = explorer->GetSelectedFile();
        if (f.IsMaterialAsset())
        {
            Material *mat = AssetsManager::LoadAsset<Material>(f.GetRelativePath());
            if (mat)
            {
                if (m_lastGameObjectOvered != overedGo)
                {
                    if (overedGo)
                    {
                        overedGo->OnDragEnterMaterial(mat);
                    }

                    if (m_lastGameObjectOvered)
                    {
                        m_lastGameObjectOvered->OnDragLeaveMaterial(mat);
                    }
                }
            }
        }
        else if (f.IsPrefabAsset())
        {
            HandleGameObjectDragging(e, explorer);
        }
    }
    else if (e->source() == hierarchy)
    {
        HandleGameObjectDragging(e, hierarchy);
    }

    m_lastGameObjectOvered = overedGo;
}

void Screen::dragLeaveEvent(QDragLeaveEvent *e)
{
    if (m_gameObjectBeingDragged)
    {
        m_lastGameObjectOvered = nullptr;
        m_gameObjectBeingDragged->SetParent(nullptr);
    }

    e->accept();
}

void Screen::dropEvent(QDropEvent *e)
{
    // NONULL(m_activeScene);
    e->ignore();
}

void Screen::OnDrop(const DragDropInfo &ddi)
{
    Scene *activeScene = SceneManager::GetActiveScene();

    if ( MouseOverMe() )
    {
        if (m_gameObjectBeingDragged)
        {
            m_gameObjectBeingDragged->m_isDragged = false;
            m_gameObjectBeingDragged->SetParent(activeScene);
            Hierarchy::GetInstance()->SelectGameObject(m_gameObjectBeingDragged);
        }
    }
    else
    {
        if (m_gameObjectBeingDragged)
        {
            delete m_gameObjectBeingDragged;
        }
    }

    m_lastGameObjectOvered = nullptr;
    m_gameObjectBeingDragged = nullptr;
}
#endif

void Screen::wheelEvent(QWheelEvent *event)
{
    Input::GetInstance()->EnqueueEvent(event);
    QGLWidget::wheelEvent(event);
}

void Screen::mouseMoveEvent(QMouseEvent *event)
{
    Input::GetInstance()->EnqueueEvent(event);
    QGLWidget::mouseMoveEvent(event);
}

void Screen::mousePressEvent(QMouseEvent *event)
{
    Input::GetInstance()->EnqueueEvent(event);
    QGLWidget::mousePressEvent(event);
    setFocus();
}

void Screen::mouseReleaseEvent(QMouseEvent *event)
{
    Input::GetInstance()->EnqueueEvent(event);
    QGLWidget::mouseReleaseEvent(event);
}

void Screen::keyPressEvent(QKeyEvent *event)
{
    Input::GetInstance()->EnqueueEvent(event);
    QGLWidget::keyPressEvent(event);
}

void Screen::keyReleaseEvent(QKeyEvent *event)
{
    Input::GetInstance()->EnqueueEvent(event);
    QGLWidget::keyReleaseEvent(event);
}
