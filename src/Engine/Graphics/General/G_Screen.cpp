#include "Bang/G_Screen.h"

#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/BFile.h"
#include "Bang/Prefab.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/SingletonManager.h"
#include "Bang/GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "Bang/Explorer.h"
#include "Bang/Hierarchy.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/SelectionFramebuffer.h"
#else
#include "Bang/GameWindow.h"
#endif

G_Screen::G_Screen(QWidget* parent)
    : QGLWidget(parent),
      m_copyShortcut     (this, KSeq("Ctrl+C"), SLOT(OnCopyClicked())),
      m_pasteShortcut    (this, KSeq("Ctrl+V"), SLOT(OnPasteClicked())),
      m_duplicateShortcut(this, KSeq("Ctrl+D"), SLOT(OnDuplicateClicked())),
      m_deleteShortcut   (this, KSeq("Del"),    SLOT(OnDeleteClicked()))
{
    setFormat(QGLFormat(QGL::DoubleBuffer));
    setAutoBufferSwap(false);
}

G_Screen::~G_Screen()
{
    if (m_gPipeline) { delete m_gPipeline; }
}

void G_Screen::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    GL::Enable(GL_DEPTH_TEST);
    GL::Enable(GL_STENCIL_TEST);
    GL::Enable(GL_CULL_FACE);

    static bool firstInitializeGL = true;
    if (firstInitializeGL)
    {
        m_gPipeline = new GraphicPipeline(this);
        firstInitializeGL = false;
    }
}

void G_Screen::paintGL()
{
    Render();
}

void G_Screen::Render()
{
    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        m_gPipeline->RenderScene(activeScene, IsRenderingInGame());
    }
    else
    {
        GL::ClearDepthBuffer();
        GL::ClearColorBuffer(Color::Pink);
    }
}

void G_Screen::resizeGL(int w, int h)
{
    GL::SetViewport(0, 0, w, h);
    m_width = w;
    m_height = h;
    m_aspectRatio = float(w) / h;

    m_gPipeline->OnResize(w, h);

    Scene *activeScene = SceneManager::GetActiveScene();
    if (activeScene)
    {
        activeScene->_OnResize(w,h);
    }
}

bool G_Screen::IsRenderingInGame() const
{
    #ifdef BANG_EDITOR
    EditorWindow *win = EditorWindow::GetInstance();
	if (!win) { return false; }

    return win->tabContainerSceneGame->currentWidget() ==
           win->tabGame;
    #else
    return true;
    #endif
}

G_Screen *G_Screen::GetInstance()
{
    IWindow *w = IWindow::GetInstance();
    #ifdef BANG_EDITOR
    return w ? Object::SCast<EditorWindow>(w)->screen : nullptr;
    #else
    return w ? Object::SCast<GameWindow>(w)->screen : nullptr;
    #endif
}

float G_Screen::GetAspectRatio()
{
    return G_Screen::GetInstance()->m_aspectRatio;
}

int G_Screen::GetWidth()
{
    return G_Screen::GetInstance()->m_width;
}

Vector2 G_Screen::GetSize()
{
    return Vector2(G_Screen::GetWidth(), G_Screen::GetHeight());
}

Vector2 G_Screen::GetPixelClipSize()
{
    return 1.0f / G_Screen::GetSize();
}

bool G_Screen::IsMouseOver()
{
    return G_Screen::GetInstance()->underMouse();
}

int G_Screen::GetHeight()
{
    return G_Screen::GetInstance()->m_height;
}

GraphicPipeline *G_Screen::GetGraphicPipeline() const
{
    return m_gPipeline;
}

#ifdef BANG_EDITOR
void G_Screen::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
}

void G_Screen::HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin)
{
    Scene *activeScene = SceneManager::GetActiveScene();
    EditorScene *scene = Object::SCast<EditorScene>(activeScene);
    SelectionFramebuffer *sfb = GraphicPipeline::GetActive()->
                                                    GetSelectionFramebuffer();
    int x = e->pos().x();
    int y = G_Screen::GetHeight() - e->pos().y();
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
                m_gameObjectBeingDragged = selectedGo->Clone();
            }
        }
        else if (origin == explorer)
        {
            BFile f = explorer->GetSelectedFile();
            if (f.IsPrefabFile())
            {
                Prefab prefab;
                prefab.ReadFromFile( f.GetPath() );
                m_gameObjectBeingDragged = prefab.InstantiateWithoutStarting();
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
            Vector2 ndcPos = Vector2(x, G_Screen::GetHeight() - y);
            ndcPos /= G_Screen::GetSize();
            ndcPos = ndcPos * 2.0f - 1.0f;
            ndcPos.y *= -1.0f;
            float z = Vector3::Distance(cam->transform->GetPosition(),
                                        scene->GetAABBox().GetCenter());
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

void G_Screen::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();

    Scene *activeScene = SceneManager::GetActiveScene();
    ENSURE(activeScene);

    SelectionFramebuffer *sfb =
            GraphicPipeline::GetActive()->GetSelectionFramebuffer();
    int x = e->pos().x(), y = G_Screen::GetHeight() - e->pos().y();
    GameObject *overedGo = sfb->GetGameObjectInPosition(x, y);

    Explorer *explorer = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (e->source() == explorer)
    {
        BFile f = explorer->GetSelectedFile();
        if (f.IsMaterialFile())
        {
            Material *mat = AssetsManager::Load<Material>(f.GetPath());
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
        else if (f.IsPrefabFile())
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

void G_Screen::dragLeaveEvent(QDragLeaveEvent *e)
{
    if (m_gameObjectBeingDragged)
    {
        m_lastGameObjectOvered = nullptr;
        m_gameObjectBeingDragged->SetParent(nullptr);
    }

    e->accept();
}

void G_Screen::dropEvent(QDropEvent *e)
{
    // ENSURE(m_activeScene);
    e->ignore();
}

void G_Screen::OnDrop(const DragDropInfo &ddi)
{
    Scene *activeScene = SceneManager::GetActiveScene();

    if ( IsMouseOverMe() )
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

void G_Screen::OnCopyClicked()
{
#ifdef BANG_EDITOR
    Hierarchy::GetInstance()->m_hContextMenu.OnCopyClicked();
#endif
}
void G_Screen::OnPasteClicked()
{
#ifdef BANG_EDITOR
    Hierarchy::GetInstance()->m_hContextMenu.OnPasteClicked();
#endif
}
void G_Screen::OnDuplicateClicked()
{
#ifdef BANG_EDITOR
    Hierarchy::GetInstance()->m_hContextMenu.OnDuplicateClicked();
#endif
}
void G_Screen::OnDeleteClicked()
{
#ifdef BANG_EDITOR
    Hierarchy::GetInstance()->m_hContextMenu.OnDeleteClicked();
#endif
}

