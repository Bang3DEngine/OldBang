#include "Bang/EditorScreen.h"

#include "Bang/BFile.h"
#include "Bang/Prefab.h"
#include "Bang/Camera.h"
#include "Bang/Explorer.h"
#include "Bang/Material.h"
#include "Bang/Hierarchy.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/EditorScene.h"
#include "Bang/EditorWindow.h"
#include "Bang/SceneManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/SelectionFramebuffer.h"

EditorScreen::EditorScreen(QWidget *parent)
    : Screen(parent),
      m_copyShortcut     (this, KSeq("Ctrl+C"), SLOT(OnCopyClicked())),
      m_pasteShortcut    (this, KSeq("Ctrl+V"), SLOT(OnPasteClicked())),
      m_duplicateShortcut(this, KSeq("Ctrl+D"), SLOT(OnDuplicateClicked())),
      m_deleteShortcut   (this, KSeq("Del"),    SLOT(OnDeleteClicked()))
{
}

EditorScreen::~EditorScreen()
{
}

void EditorScreen::Render() const
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

bool EditorScreen::IsRenderingInGame() const
{
    EditorWindow *win = EditorWindow::GetInstance();
        if (!win) { return false; }

    return win->tabContainerSceneGame->currentWidget() ==
           win->tabGame;
}

bool EditorScreen::IsMouseOver() const
{
    return Screen::GetInstance()->underMouse();
}

void EditorScreen::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept();
}

void EditorScreen::HandleGameObjectDragging(QDragMoveEvent *e, QWidget *origin)
{
    Scene *activeScene = SceneManager::GetActiveScene();
    EditorScene *scene = Object::SCast<EditorScene>(activeScene);
    SelectionFramebuffer *sfb = GraphicPipeline::GetActive()->
                                                    GetSelectionFramebuffer();
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
            Vector2 ndcPos = Vector2(x, Screen::GetHeight() - y);
            ndcPos /= Screen::GetSize();
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

void EditorScreen::dragMoveEvent(QDragMoveEvent *e)
{
    e->accept();

    Scene *activeScene = SceneManager::GetActiveScene();
    ENSURE(activeScene);

    SelectionFramebuffer *sfb =
            GraphicPipeline::GetActive()->GetSelectionFramebuffer();
    int x = e->pos().x(), y = Screen::GetHeight() - e->pos().y();
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

void EditorScreen::dragLeaveEvent(QDragLeaveEvent *e)
{
    if (m_gameObjectBeingDragged)
    {
        m_lastGameObjectOvered = nullptr;
        m_gameObjectBeingDragged->SetParent(nullptr);
    }

    e->accept();
}

void EditorScreen::dropEvent(QDropEvent *e)
{
    e->ignore();
}

void EditorScreen::OnDrop(const DragDropInfo &ddi)
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

void EditorScreen::OnCopyClicked()
{
    Hierarchy::GetInstance()->m_hContextMenu.OnCopyClicked();
}
void EditorScreen::OnPasteClicked()
{
    Hierarchy::GetInstance()->m_hContextMenu.OnPasteClicked();
}
void EditorScreen::OnDuplicateClicked()
{
    Hierarchy::GetInstance()->m_hContextMenu.OnDuplicateClicked();
}
void EditorScreen::OnDeleteClicked()
{
    Hierarchy::GetInstance()->m_hContextMenu.OnDeleteClicked();
}
