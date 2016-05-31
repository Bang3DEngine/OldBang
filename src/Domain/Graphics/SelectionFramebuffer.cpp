#include "SelectionFramebuffer.h"

SelectionFramebuffer::SelectionFramebuffer(int width, int height) :
    Framebuffer(width, height)
{
    program = new ShaderProgram(
                ShaderContract::Filepath_Shader_Vertex_PVM_Position,
                ShaderContract::Filepath_Shader_Fragment_Selection);
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene)
{
    //Logger_Log("______________");
    program->Bind();
    std::list<Renderer*> childrenRenderers =
            scene->GetComponentsInChildren<Renderer>();
    for(auto it = childrenRenderers.begin(); it != childrenRenderers.end(); ++it)
    {
        Renderer *renderer = *it;
        GameObject *go = renderer->GetOwner();
        renderer->RenderWithoutBindingMaterial();
       // program->SetUniformVec4("selectionColor",
       //                         MapIdToColor(gameObjectsToId[go]));

        //Logger_Log(renderer->GetOwner()->GetName());
    }
    program->UnBind();
}

void SelectionFramebuffer::OnChildAdded(GameObject *child)
{
    ++idCount;
    gameObjectsToId[child] = idCount;
    idToGameObject[idCount] = child;

    //Logger_Log("Received " << child);
}

glm::vec4 SelectionFramebuffer::MapIdToColor(long id)
{
    return glm::vec4(id % 255,
                     id / 255,
                     (id / 255) /255,
                     (((id / 255) / 255) / 255)
                     );
}

long SelectionFramebuffer::MapColorToId(const glm::vec4 &color)
{
    return color.r * 255 +
           color.g * 255 * 255 +
           color.b * 255 * 255 * 255 +
           color.a * 255 * 255 * 255 * 255;
}
