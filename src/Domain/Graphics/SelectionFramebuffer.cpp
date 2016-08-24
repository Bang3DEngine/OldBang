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
    delete m_colorTexture;
    delete m_worldPosTexture;
    delete m_program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene)
{
    SetAllDrawBuffers();

    m_isPassing = true;

    // Assign id's
    int id = 0;
    m_gameObjectToId.clear(); m_idToGameObject.clear();
    std::list<GameObject*> gameObjects = scene->GetChildrenRecursivelyEditor();
    for (GameObject *go : gameObjects)
    {
        m_gameObjectToId[go] = id;
        m_idToGameObject[id] = go;
        ++id;
    }

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
    SetDrawBuffers({Attachment::ColorAttachment});
    glDepthMask(GL_FALSE); // Dont write to DepthBuffer
    for (GameObject *go : gameObjects)
    {
        if (CanRenderGameObject(go))
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnDrawGizmos();
        }
    }

    glDisable(GL_DEPTH_TEST);
    for (GameObject *go : gameObjects)
    {
        if (CanRenderGameObject(go))
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnDrawGizmosNoDepth();
        }
    }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE); // Back to writing DepthBuffer

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
    Debug_Log("ProcessSelection");
    if (Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        Debug_Log("GetMouseButtonDown YES");
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

Color SelectionFramebuffer::MapIdToColor(long id)
{
    Color color =
            Color(
                    double(  id               % 256),
                    double( (id / 256)        % 256),
                    double(((id / 256) / 256) % 256)
                   );

   return color / 256.0f;
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
