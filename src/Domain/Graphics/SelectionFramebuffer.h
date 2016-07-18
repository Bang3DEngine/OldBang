#ifndef SELECTIONFRAMEBUFFER_H
#define SELECTIONFRAMEBUFFER_H

#include <map>

#include "Scene.h"
#include "Framebuffer.h"
#include "GameObject.h"

/**
 * @brief This is the buffer where each GameObject is drawn into with a unique colour.
 */
class SelectionFramebuffer : public Framebuffer
                            #ifdef BANG_EDITOR
                            ,public IWindowEventManagerListener
                            #endif
{
private:
    ShaderProgram *m_program = nullptr;
    Material *m_material = nullptr;

    /**
     * @brief Keeps track of the last GameObject the mouse was over.
     * This lets us inform of mouseOut events.
     */
    GameObject *p_lastMouseOverGO = nullptr;

    std::map<GameObject*, long> m_gameObjectToId;
    std::map<long, GameObject*> m_idToGameObject;

    static Vector3 MapIdToColor(long id);
    static long MapColorToId(const Vector3 &charColor);

public:
    SelectionFramebuffer(int width, int height);
    virtual ~SelectionFramebuffer();

    void RenderSelectionBuffer(const Scene *scene);
    void ProcessSelection();
};

#endif // SELECTIONFRAMEBUFFER_H
