#include "SelectionFramebuffer.h"

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
    //Logger_Log("______________");
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
        renderer->RenderWithoutBindingMaterial();
        program->SetUniformVec4("selectionColor",
                                MapIdToColor(gameObjectsToId[go]));

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
    return glm::vec4(double( (id % 255) / 255.0d),
                     double(id / 255.0d),
                     double((id / 255.0d) / 255.0d),
                     double((((id / 255.0d) / 255.0d) / 255.0d))
                     );
}

long SelectionFramebuffer::MapColorToId(const glm::vec4 &color)
{
    return color.r * 255 +
           color.g * 255 * 255 +
           color.b * 255 * 255 * 255 +
           color.a * 255 * 255 * 255 * 255;
}
