#ifndef SELECTIONFRAMEBUFFER_H
#define SELECTIONFRAMEBUFFER_H

#include <map>

#include "Scene.h"
#include "Framebuffer.h"
#include "GameObject.h"

/**
 * @brief This is the buffer where each GameObject is drawn into with a unique colour.
 */
class SelectionFramebuffer : public Framebuffer,
                             public IWindowEventManagerListener
{
private:
    ShaderProgram *program;

    long idCount = 0;
    std::map<GameObject, long> gameObjectsToId;
    std::map<long, GameObject> idToGameObject;

    static glm::vec4 MapIdToColor(long id);
    static long MapColorToId(const glm::vec4 &color);

public:
    SelectionFramebuffer();
    virtual ~SelectionFramebuffer();

    void RenderSelectionBuffer(const Scene *scene) const;

    void OnChildAdded(GameObject *child) override;
};

#endif // SELECTIONFRAMEBUFFER_H
