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
        if (go->IsEnabled())
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go));
            go->_OnRender();
        }
    }

    for (GameObject *go : gameObjects)
    {
        if (go->IsEnabled())
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go));
            go->_OnDrawGizmos();
        }
    }

    glClear(GL_DEPTH_BUFFER_BIT);
    for (GameObject *go : gameObjects)
    {
        if (go->IsEnabled())
        {
            sp->SetUniformVec3("selectionColor", GetSelectionColor(go));
            go->_OnDrawGizmosNoDepth();
        }
    }

    m_isPassing = false;
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    glm::vec2 coords = Input::GetMouseCoords();
    coords = glm::vec2(coords.x, Canvas::GetHeight()-coords.y);
    Vector3 mouseOverColor = ReadPixel(coords.x, coords.y, 0);

    GameObject *mouseOverGO = nullptr;
    int id = MapColorToId(mouseOverColor);
    if (m_idToGameObject.find(id) != m_idToGameObject.end())
    {
        mouseOverGO = m_idToGameObject[id];
    }
    //Logger_Log(mouseOverGO);

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

Vector3 SelectionFramebuffer::GetSelectionColor(GameObject *go) const
{
    return MapIdToColor(m_gameObjectToId[go]);
}

Material *SelectionFramebuffer::GetSelectionMaterial() const
{
    return m_material;
}

bool SelectionFramebuffer::IsPassing() const
{
    return m_isPassing;
}

Vector3 SelectionFramebuffer::MapIdToColor(long id)
{
    const int C = 256;
    Vector3 charColor =
            Vector3(
                    double(id % C),
                    double((id / C) % C),
                    double(((id / C) / C) % C)
                   );

   return charColor / C;
}

long SelectionFramebuffer::MapColorToId(const Vector3 &charColor)
{
    const int C = 256;
    Vector3 color = charColor / 256.0d;
    return long(color.r * C) +
           long(color.g * C * C) +
           long(color.b * C * C * C); // This +1 is because of the TextureRender GL_RGBA32F format. If its GL_RGB, then we dont need it :/
}
