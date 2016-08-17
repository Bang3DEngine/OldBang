#include "SelectionFramebuffer.h"

#include "WindowMain.h"

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

    CreateColorAttachment(0, GL_RGB, GL_RGB);
    // CreateColorAttachment(1, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, true);
    CreateColorAttachment(1, GL_RGBA, GL_RGBA, GL_FLOAT);
    CreateDepthBufferAttachment();
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete m_program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene)
{
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
        if (go->IsEnabled() && !go->IsDraggedGameObject())
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnRender();
        }
    }

    /*
    glDepthMask(GL_FALSE); // Dont write to DepthBuffer
    for (GameObject *go : gameObjects)
    {
        if (go->IsEnabled() && !go->IsDraggedGameObject())
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnDrawGizmos();
        }
    }

    glDisable(GL_DEPTH_TEST);
    for (GameObject *go : gameObjects)
    {
        if (go->IsEnabled() && !go->IsDraggedGameObject())
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go).ToVector3());
            go->_OnDrawGizmosNoDepth();
        }
    }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE); // Back to writing DepthBuffer
    */

    m_isPassing = false;
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    Vector2 coords = Input::GetMouseCoords();
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
                WindowMain::GetInstance()->widgetHierarchy->SelectGameObject(mouseOverGO);
                if (Input::GetMouseButtonDoubleClick(Input::MouseButton::MLeft)) // Double clicking
                {
                    WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(mouseOverGO);
                }
            }
        }
        else // Background has been pressed
        {
            WindowMain::GetInstance()->widgetHierarchy->UnselectAll();
        }
    }
}

GameObject *SelectionFramebuffer::GetGameObjectInPosition(int x, int y)
{
    Vector2 coords = Vector2(x, Canvas::GetHeight() - y);
    Color mouseOverColor255  = ReadColor255(coords.x, coords.y, 0);

    int id = MapColorToId(mouseOverColor255);
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

float SelectionFramebuffer::GetDepthAt(int x, int y)
{
    // 1 is depthAttachment in this case
    // float d = ReadFloat(x, y, 1, GL_DEPTH_COMPONENT, GL_FLOAT);
    float d = ReadColor(x, y, 1).r;
    return d;
}

bool SelectionFramebuffer::IsPassing() const
{
    return m_isPassing;
}

Color SelectionFramebuffer::MapIdToColor(long id)
{
    const int C = 256;
    Color charColor =
            Color(
                    double(id % C),
                    double((id / C) % C),
                    double(((id / C) / C) % C)
                   );

   return charColor / C;
}

long SelectionFramebuffer::MapColorToId(const Color &charColor)
{
    const int C = 256;
    Color color = charColor / 256.0d;
    return long(color.r * C) +
           long(color.g * C * C) +
           long(color.b * C * C * C); // This +1 is because of the TextureRender GL_RGBA32F format. If its GL_RGB, then we dont need it :/
}
