#include "Bang/SelectionFramebuffer.h"

#include "Bang/IO.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Cursor.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/GameObject.h"
#include "Bang/ShaderProgram.h"
#include "Bang/RenderTexture.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/Hierarchy.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/WindowEventManager.h"
#endif

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    m_selectionProgram = new ShaderProgram(
                                    EPATH("Shaders/G_Default.vert_g"),
                                    EPATH("Shaders/SelectionBuffer.frag_sel"));

    CreateColorAttachment(AttColor,    Texture::Format::RGBA_Float16);
    CreateColorAttachment(AttWorldPos, Texture::Format::RGBA_Float16);
    CreateDepthRenderbufferAttachment();

    m_colorTexture    = GetAttachmentTexture(AttColor);
    m_worldPosTexture = GetAttachmentTexture(AttWorldPos);
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete m_selectionProgram;
}

void SelectionFramebuffer::PrepareForRender(const Scene *scene)
{
    int id = 0;
    m_gameObject_To_Id.Clear();
    m_id_To_GameObject.Clear();
    List<GameObject*> gameObjects = scene->GetChildrenRecursively();
    for (GameObject *go : gameObjects)
    {
        m_gameObject_To_Id[go] = id;
        m_id_To_GameObject[id] = go;
        ++id;
    }
}

void SelectionFramebuffer::PrepareNextGameObject(GameObject *go)
{
    m_nextGameObjectToBeRendered = go;
}

void SelectionFramebuffer::RenderForSelectionBuffer(Renderer *rend)
{
    SetAllDrawBuffers();

    GameObject *go = !Gizmos::IsGizmoRenderer(rend) ?
                rend->gameObject : m_nextGameObjectToBeRendered;
    if (CanRenderGameObject(go))
    {
        // This should be a selection ReplacementShader put by the GP
        Material *rendMaterial = rend->GetMaterial();
        ShaderProgram *sp = rendMaterial->GetShaderProgram();
        sp->SetColor("selectionColor", GetSelectionColor(go));

        rend->Render();
    }
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    Vector2 coords = Input::GetMouseCoords();
    coords.y = Screen::GetHeight() - coords.y;
    GameObject *mouseOverGO = GetGameObjectInPosition(coords.x, coords.y);
    if (!Screen::MouseOver()) { mouseOverGO = nullptr; }

    if (m_lastMouseOverGO  && m_lastMouseOverGO != mouseOverGO)
    {
        m_lastMouseOverGO->OnMouseExit(false);
    }

    if (mouseOverGO)
    {
        if (m_lastMouseOverGO != mouseOverGO)
        {
            mouseOverGO->OnMouseEnter(false);
        }

        mouseOverGO->OnMouseOver(false);
    }

    m_lastMouseOverGO = mouseOverGO;

    // Selection (clicking over) Here we just handle non-GameObjects
    if (Input::GetMouseButtonDown(Input::MouseButton::Left))
    {
        if (mouseOverGO)
        {
            // Selection of a GameObject
            if (!mouseOverGO->GetHideFlags()->IsOn(HideFlag::HideInHierarchy))
            {
                EditorWindow::GetInstance()->
                        widgetHierarchy->SelectGameObject(mouseOverGO);
                if (Input::GetMouseButtonDoubleClick(Input::MouseButton::Left))
                {
                    // Double clicking
                    WindowEventManager::
                            NotifyHierarchyGameObjectDoubleClicked(mouseOverGO);
                }
            }
        }
        else // Background has been pressed
        {
            EditorWindow::GetInstance()->widgetHierarchy->UnselectAll();
        }
    }
}

GameObject *SelectionFramebuffer::GetGameObjectInPosition(int x, int y)
{
    Color mouseOverColor = ReadColor(x, y, AttColor);
    int id = MapColorToId(mouseOverColor);
    if (mouseOverColor != Color::Zero &&
        m_id_To_GameObject.ContainsKey(id))
    {
        return m_id_To_GameObject[id];
    }
    return nullptr;
}

Color SelectionFramebuffer::GetSelectionColor(GameObject *go) const
{
    return MapIdToColor(m_gameObject_To_Id[go]);
}

Vector3 SelectionFramebuffer::GetWorldPositionAt(int x, int y)
{
    return ReadColor(x, y, AttWorldPos).ToVector3();
}

bool SelectionFramebuffer::IsPassing() const
{
    return m_isPassing;
}

ShaderProgram *SelectionFramebuffer::GetSelectionShaderProgram() const
{
    return m_selectionProgram;
}

void SelectionFramebuffer::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (destroyed == m_lastMouseOverGO)
    {
        Cursor::SetIcon( Cursor::CursorIcon::Arrow );
        m_lastMouseOverGO = nullptr;
    }
}

Color SelectionFramebuffer::MapIdToColor(long id)
{
    const int C = 256;
    Color color =
            Color(
                    double(  id           % C),
                    double( (id / C)      % C),
                    double(((id / C) / C) % C)
                   );
   return color / float(C);
}

long SelectionFramebuffer::MapColorToId(const Color &color)
{
    const int C = 256;
    return long(color.r * C) +
           long(color.g * C * C) +
           long(color.b * C * C * C);
}

bool SelectionFramebuffer::CanRenderGameObject(const GameObject *go)
{
    if (!go) { return false; }
    if (go->GetHideFlags().IsOn(HideFlag::HideInHierarchy))
    {
       return go->IsEnabled() && !go->IsDraggedGameObject() &&
              go->GetHideFlags().IsOn(HideFlag::HideInSelection);
    }
    return go->IsEnabled() && !go->IsDraggedGameObject();
}


RenderTexture *SelectionFramebuffer::GetColorTexture() const
{
    return m_colorTexture;
}

RenderTexture *SelectionFramebuffer::GetWorldPosTexture() const
{
    return m_worldPosTexture;
}
