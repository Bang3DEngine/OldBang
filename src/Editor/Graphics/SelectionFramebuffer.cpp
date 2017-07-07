#include "Bang/SelectionFramebuffer.h"

#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Cursor.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Hierarchy.h"
#include "Bang/GameObject.h"
#include "Bang/G_Material.h"
#include "Bang/EditorScreen.h"
#include "Bang/EditorWindow.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/G_RenderTexture.h"
#include "Bang/WindowEventManager.h"

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    G_Framebuffer(width, height)
{
    ShaderProgram *selectionProgram = new ShaderProgram();
    selectionProgram->Load( EPATH("Shaders/G_Default.vert_g"),
                            EPATH("Shaders/SelectionBuffer.frag_sel") );

    m_selectionMaterial = new Material();
    m_selectionMaterial->SetShaderProgram(selectionProgram);

    CreateColorAttachment(AttColor,    G_Texture::Format::RGBA_Float16);
    CreateColorAttachment(AttWorldPos, G_Texture::Format::RGBA_Float16);
    CreateDepthRenderbufferAttachment();

    m_colorTexture    = GetAttachmentTexture(AttColor);
    m_worldPosTexture = GetAttachmentTexture(AttWorldPos);
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete m_selectionMaterial;
}

void SelectionFramebuffer::PrepareForRender(const Scene *scene)
{
    int id = 1;
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
    ASSERT(GL::IsBound(this));
    SetAllDrawBuffers();

    GameObject *go = !Gizmos::IsGizmoRenderer(rend) ?
                rend->gameObject : m_nextGameObjectToBeRendered;
    if (CanRenderGameObject(go))
    {
        Material *previousMaterial = rend->GetMaterial();
        rend->SetMaterial(m_selectionMaterial, false);
        rend->Bind();
        G_ShaderProgram *selSP = m_selectionMaterial->GetShaderProgram();
        selSP->SetColor("selectionColor", GetSelectionColor(go));
        rend->Render();
        rend->UnBind();
        rend->SetMaterial(previousMaterial);
    }
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    Vector2 coords = Input::GetMouseCoords();
    coords.y = Screen::GetHeight() - coords.y;
    GameObject *mouseOverGO = GetGameObjectInPosition(coords.x, coords.y);
    EditorScreen *screen = static_cast<EditorScreen*>(Screen::GetInstance());
    if (!screen->IsMouseOver()) { mouseOverGO = nullptr; }

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
    if (mouseOverColor != Color::Zero && m_id_To_GameObject.ContainsKey(id))
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
    constexpr int C = 256;
    Color color =
            Color(
                    double(   id                % C),
                    double(  (id / C)           % C),
                    double( ((id / C) / C)      % C),
                    double((((id / C) / C) / C) % C)
                   );
   return color / float(C);
}

long SelectionFramebuffer::MapColorToId(const Color &color)
{
    constexpr int C = 256;
    return long(color.r * C) +
           long(color.g * C * C) +
           long(color.b * C * C * C) +
           long(color.a * C * C * C * C);
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


G_RenderTexture *SelectionFramebuffer::GetColorTexture() const
{
    return m_colorTexture;
}

G_RenderTexture *SelectionFramebuffer::GetWorldPosTexture() const
{
    return m_worldPosTexture;
}
