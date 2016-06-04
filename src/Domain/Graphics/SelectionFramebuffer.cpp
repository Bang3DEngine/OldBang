#include "SelectionFramebuffer.h"

#include "WindowMain.h"
#include "Hierarchy.h"

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    program = new ShaderProgram(
                ShaderContract::Filepath_Shader_Vertex_PVM_Position,
                ShaderContract::Filepath_Shader_Fragment_Selection);

    CreateTextureAttachment(0, AttachmentType::Color);
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene)
{
    Bind();

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->Bind();
    std::list<Renderer*> childrenRenderers =
            scene->GetComponentsInChildren<Renderer>();
    for(auto it = childrenRenderers.begin(); it != childrenRenderers.end(); ++it)
    {
        Renderer *renderer = *it;
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

        GameObject *go = renderer->GetOwner();
        if(gameObjectToId.find(go) != gameObjectToId.end())
        {
            Vector3 selectionColor = MapIdToColor(gameObjectToId[go]);
            program->SetUniformVec3("selectionColor", selectionColor);
            renderer->RenderWithoutBindingMaterial();
        }
    }
    program->UnBind();

    UnBind();
}

void SelectionFramebuffer::ProcessSelection()
{
    //Get mouse coordinates and read pixel color
    glm::vec2 coords = Input::GetMouseCoords();
    Vector3 mouseOverColor = ReadPixel(coords.x, Canvas::GetHeight()-coords.y, 0);

    GameObject *mouseOverGO = nullptr;
    //If color over is not the background (255, 255, 255)
    if(mouseOverColor.r < 254 || mouseOverColor.g < 254 || mouseOverColor.b < 254)
    {
        int id = MapColorToId(mouseOverColor);
        if(idToGameObject.find(id) != idToGameObject.end())
        {
            mouseOverGO = idToGameObject[id];
        }
    }

    // MouseOver and MouseOut events
    if(lastMouseOverGO != nullptr && lastMouseOverGO != mouseOverGO)
    {
        lastMouseOverGO->OnMouseOut();
    }

    if(mouseOverGO != nullptr)
    {
        mouseOverGO->OnMouseOver();
        lastMouseOverGO = mouseOverGO;
    }
    //

    // Selection (clicking over) Here we just handle non-EditorGameObjects
    if(Input::GetMouseButtonDown(Input::MouseButton::MLeft))
    {
        if(mouseOverGO != nullptr)
        {
            if(!mouseOverGO->IsEditorGameObject()) //Selection of a GameObject
            {
                WindowMain::GetInstance()->widgetHierarchy->SelectGameObject(mouseOverGO);
                if(Input::GetMouseButtonDoubleClick(Input::MouseButton::MLeft)) // Double clicking
                {
                    WindowEventManager::NotifyHierarchyGameObjectDoubleClicked(mouseOverGO);
                }
            }
        }
        else //Background has been pressed
        {
            WindowMain::GetInstance()->widgetHierarchy->UnselectAll();
        }
    }
    //
}

void SelectionFramebuffer::OnChildAdded(GameObject *child)
{
    gameObjectToId[child] = idCount;
    idToGameObject[idCount] = child;
    ++idCount;
}

Vector3 SelectionFramebuffer::MapIdToColor(long id)
{
    Vector3 charColor =
            Vector3(
                    double(id % 256),
                    double((id / 256) % 256),
                    double(((id / 256) / 256) % 256)
                   );

   return charColor / 256.0d;
}

long SelectionFramebuffer::MapColorToId(const Vector3 &charColor)
{
    Vector3 color = charColor / 256.0d;
    return long(color.r * 256) +
           long(color.g * 256 * 256) +
           long(color.b * 256 * 256 * 256);
}
