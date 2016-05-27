#include "SelectionFramebuffer.h"

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

SelectionFramebuffer::SelectionFramebuffer()
{
    program = new ShaderProgram(
                ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal,
                ShaderContract::Filepath_Shader_Fragment_Selection);
}

SelectionFramebuffer::~SelectionFramebuffer()
{
    delete program;
}

void SelectionFramebuffer::RenderSelectionBuffer(const Scene *scene) const
{
    scene->_OnRender();

    /*
    std::list<GameObject*> children = scene->GetChildren();
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        GameObject *go = *it;
        Renderer *renderer = nullptr;
        if( (renderer = go->GetComponent<Renderer>()) != nullptr)
        {
            renderer->OnRender();
        }
    }
    */
}

void SelectionFramebuffer::OnChildAdded(GameObject *child)
{
    ++idCount;
    gameObjectsToId[child] = idCount;
    idToGameObject[idCount] = child;

    Logger_Log("Received");
}
