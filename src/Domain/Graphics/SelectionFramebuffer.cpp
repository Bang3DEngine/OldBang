#include "SelectionFramebuffer.h"

#include "WindowMain.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#endif

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    m_program = new ShaderProgram(
                ShaderContract::Filepath_Shader_Vertex_PVM_Position,
                ShaderContract::Filepath_Shader_Fragment_Selection);

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
    m_material->Bind();

    // Assign id's
    m_gameObjectToId.clear();
    m_idToGameObject.clear();
    int id = 0;
    std::list<Renderer*> childrenRenderers = scene->GetComponentsInChildren<Renderer>();
    for(Renderer *renderer : childrenRenderers)
    {
        GameObject *go = renderer->gameObject;
        m_gameObjectToId[go] = id;
        m_idToGameObject[id] = go;
        ++id;
    }

    // Paint objects
    for(Renderer *renderer : childrenRenderers)
    {
        if(renderer->gameObject->GetRenderLayer() == scene->m_currentRenderLayer)
        {
            GameObject *go = renderer->gameObject;
            if(m_gameObjectToId.find(go) != m_gameObjectToId.end() && go->IsEnabled())
            {
                Matrix4 model, view, projection, pvm;
                renderer->GetMatrices(model, view, projection, pvm);

                m_program->SetUniformMat4(ShaderContract::Uniform_Matrix_Model,
                                        model, false);
                m_program->SetUniformMat4(ShaderContract::Uniform_Matrix_View,
                                        view, false);
                m_program->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection,
                                        projection, false);
                m_program->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM,
                                        pvm, false);

                Vector3 selectionColor = MapIdToColor(m_gameObjectToId[go]);
                m_program->SetUniformVec3("selectionColor", selectionColor);

                renderer->ActivateGLStatesBeforeRendering();
                if(renderer->ActivateGLStatesBeforeRenderingForSelection)
                    renderer->ActivateGLStatesBeforeRenderingForSelection();
                renderer->RenderWithoutBindingMaterial();
            }
        }
    }

    m_material->UnBind();
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    glm::vec2 coords = Input::GetMouseCoords();
    coords = glm::vec2(coords.x, Canvas::GetHeight()-coords.y);
    Vector3 mouseOverColor = ReadPixel(coords.x, coords.y, 0);

    GameObject *mouseOverGO = nullptr;
    int id = MapColorToId(mouseOverColor);
    if(m_idToGameObject.find(id) != m_idToGameObject.end())
    {
        mouseOverGO = m_idToGameObject[id];
    }

    if(p_lastMouseOverGO  && p_lastMouseOverGO != mouseOverGO)
    {
        p_lastMouseOverGO->OnMouseExit(false);
    }

    if(mouseOverGO)
    {
        if(p_lastMouseOverGO != mouseOverGO)
        {
            mouseOverGO->OnMouseEnter(false);
        }

        mouseOverGO->OnMouseOver(false);
    }

    p_lastMouseOverGO = mouseOverGO;


    // Selection (clicking over) Here we just handle non-EditorGameObjects
    if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        if(mouseOverGO )
        {
            if(!mouseOverGO->IsEditorGameObject()) // Selection of a GameObject
            {
                WindowMain::GetInstance()->widgetHierarchy->SelectGameObject(mouseOverGO);
                if(Input::GetMouseButtonDoubleClick(Input::MouseButton::MLeft)) // Double clicking
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
