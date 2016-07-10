#include "SelectionFramebuffer.h"

#include "WindowMain.h"
#include "Hierarchy.h"

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    program = new ShaderProgram(
                ShaderContract::Filepath_Shader_Vertex_PVM_Position,
                ShaderContract::Filepath_Shader_Fragment_Selection);

    CreateColorAttachment(0);
    CreateDepthBufferAttachment();
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene)
{
    program->Bind();

    // Assign id's
    gameObjectToId.clear();
    idToGameObject.clear();
    int id = 0;
    std::list<Renderer*> childrenRenderers = scene->GetComponentsInChildren<Renderer>();
    for(Renderer *renderer : childrenRenderers)
    {
        GameObject *go = renderer->gameObject;
        gameObjectToId[go] = id;
        idToGameObject[id] = go;
        ++id;
    }

    // Paint objects
    for(Renderer *renderer : childrenRenderers)
    {
        if(renderer->gameObject->GetRenderLayer() == scene->currentRenderLayer)
        {
            GameObject *go = renderer->gameObject;
            if(gameObjectToId.find(go) != gameObjectToId.end() && go->IsEnabled())
            {
                Matrix4 model, view, projection, pvm;
                renderer->GetMatrices(model, view, projection, pvm);

                program->SetUniformMat4(ShaderContract::Uniform_Matrix_Model,
                                        model, false);
                program->SetUniformMat4(ShaderContract::Uniform_Matrix_View,
                                        view, false);
                program->SetUniformMat4(ShaderContract::Uniform_Matrix_Projection,
                                        projection, false);
                program->SetUniformMat4(ShaderContract::Uniform_Matrix_PVM,
                                        pvm, false);

                Vector3 selectionColor = MapIdToColor(gameObjectToId[go]);
                program->SetUniformVec3("selectionColor", selectionColor);

                renderer->ActivateGLStatesBeforeRendering();
                if(renderer->ActivateGLStatesBeforeRenderingForSelection)
                    renderer->ActivateGLStatesBeforeRenderingForSelection();
                renderer->RenderWithoutBindingMaterial();
            }
        }
    }

    program->UnBind();
}

void SelectionFramebuffer::ProcessSelection()
{
    // Get mouse coordinates and read pixel color
    glm::vec2 coords = Input::GetMouseCoords();
    coords = glm::vec2(coords.x, Canvas::GetHeight()-coords.y);
    Vector3 mouseOverColor = ReadPixel(coords.x, coords.y, 0);

    GameObject *mouseOverGO = nullptr;
    int id = MapColorToId(mouseOverColor);
    if(idToGameObject.find(id) != idToGameObject.end())
    {
        mouseOverGO = idToGameObject[id];
    }

    // MouseOver and MouseOut events
    if(lastMouseOverGO  && lastMouseOverGO != mouseOverGO)
    {
        lastMouseOverGO->OnMouseExit();
    }

    if(mouseOverGO )
    {
        mouseOverGO->OnMouseEnter();
        lastMouseOverGO = mouseOverGO;
    }
    //

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
           long(color.b * C * C * C);
}
