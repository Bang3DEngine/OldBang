#include "SelectionFramebuffer.h"

#include "EditorWindow.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#endif

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    m_program = new ShaderProgram(
                ShaderContract::Filepath_Shader_SelectionBuffer_VS,
                ShaderContract::Filepath_Shader_SelectionBuffer_FS);

    m_material = new Material();
    m_material->SetShaderProgram(m_program);

    m_colorTexture = new TextureRender();
    /*
    m_colorTexture->SetGLFormat(GL_RGB);
    m_colorTexture->SetGLInternalFormat(GL_RGB);
    */
    m_worldPosTexture = new TextureRender();
    SetColorAttachment(Attachment::ColorAttachment, m_colorTexture);
    SetColorAttachment(Attachment::WorldPosition, m_worldPosTexture);
    /*
    CreateColorAttachment(Attachment::ColorAttachment, GL_RGB, GL_RGB);
    CreateColorAttachment(Attachment::WorldPosition, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    */
    CreateDepthRenderbufferAttachment();
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete m_program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene)
{
    Chrono c("SelectionFramebuffer");
    c.MarkEvent("Init");
    SetAllDrawBuffers();

    m_isPassing = true;

    // Assign id's
    c.MarkEvent("Assign Ids");
    int id = 0;
    m_gameObjectToId.clear(); m_idToGameObject.clear();
    std::list<GameObject*> gameObjects = scene->GetChildrenRecursivelyEditor();
    for (GameObject *go : gameObjects)
    {
        m_gameObjectToId[go] = id;
        m_idToGameObject[id] = go;
        ++id;
    }

    c.MarkEvent("Normal pass");
    ShaderProgram *sp = m_material->GetShaderProgram();
    for (GameObject *go : gameObjects)
    {
        if (CanRenderGameObject(go))
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnRender();
        }
    }

    // Dont write to Attachment::WorldPosition
    c.MarkEvent("Gizmos pass");
    SetDrawBuffers({Attachment::ColorAttachment});
    for (GameObject *go : gameObjects)
    {
        if (CanRenderGameObject(go))
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnDrawGizmos();
        }
    }

    c.MarkEvent("GizmosND pass");
    glClear(GL_DEPTH_BUFFER_BIT);
    for (GameObject *go : gameObjects)
    {
        if (CanRenderGameObject(go))
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnDrawGizmosNoDepth();
        }
    }

    c.Log();

    m_isPassing = false;
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    Vector2 coords = Input::GetMouseCoords();
    coords.y = Screen::GetHeight() - coords.y;
    GameObject *mouseOverGO = GetGameObjectInPosition(coords.x, coords.y);

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

    // Selection (clicking over) Here we just handle non-EditorGameObjects
    if (Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        if (mouseOverGO)
        {
            if (!mouseOverGO->IsEditorGameObject()) // Selection of a GameObject
            {
                EditorWindow::GetInstance()->widgetHierarchy->SelectGameObject(mouseOverGO);
                if (Input::GetMouseButtonDoubleClick(Input::MouseButton::MLeft)) // Double clicking
                {
                    WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(mouseOverGO);
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
    Color mouseOverColor = ReadColor(x, y, Attachment::ColorAttachment);
    int id = MapColorToId(mouseOverColor);
    if (m_idToGameObject.find(id) != m_idToGameObject.end())
    {
        return m_idToGameObject[id];
    }
    return nullptr;
}

Color SelectionFramebuffer::GetSelectionColor(GameObject *go) const
{
    return MapIdToColor(m_gameObjectToId[go]);
}

Material *SelectionFramebuffer::GetSelectionMaterial() const
{
    return m_material;
}

Vector3 SelectionFramebuffer::GetWorldPositionAt(int x, int y)
{
    return ReadColor(x, y, Attachment::WorldPosition).ToVector3();
}

bool SelectionFramebuffer::IsPassing() const
{
    return m_isPassing;
}

void SelectionFramebuffer::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (destroyed == m_lastMouseOverGO)
    {
        m_lastMouseOverGO = nullptr;
    }
}

Color SelectionFramebuffer::MapIdToColor(long id)
{
    const int C = 256;
    Color color =
            Color(
                    double(  id               % C),
                    double( (id / C)        % C),
                    double(((id / C) / C) % C)
                   );

   return color / float(C);
}

long SelectionFramebuffer::MapColorToId(const Color &color)
{
    return long(color.r * 256) +
           long(color.g * 256 * 256) +
           long(color.b * 256 * 256 * 256);
}

bool SelectionFramebuffer::CanRenderGameObject(const GameObject *go)
{
    if (go->IsEditorGameObject())
    {
       const EditorGameObject *ego = static_cast<const EditorGameObject*>(go);
       return ego->IsEnabled() && !ego->IsDraggedGameObject() &&
              ego->IsRenderInSelectionFramebuffer();
    }
    return go->IsEnabled() && !go->IsDraggedGameObject();
}
