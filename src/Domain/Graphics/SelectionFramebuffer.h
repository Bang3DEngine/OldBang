#ifndef SELECTIONFRAMEBUFFER_H
#define SELECTIONFRAMEBUFFER_H

#include <map>

#include "Scene.h"
#include "Framebuffer.h"
#include "Vector3.h"
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

    /**
     * @brief m_isPassing is true only when SelectionFrambuffer is rendering.
     * It is used by the Renderer's, to know if they have to activate its special states
     * before rendering.
     */
    bool m_isPassing;

    ShaderProgram *m_program = nullptr;
    Material *m_material = nullptr;

    /**
     * @brief Keeps track of the last GameObject the mouse was over.
     * This lets us inform of mouseOut events.
     */
    GameObject *m_lastMouseOverGO = nullptr;

    mutable std::map<GameObject*, long> m_gameObjectToId;
    mutable std::map<long, GameObject*> m_idToGameObject;

    static Vector3 MapIdToColor(long id);
    static long MapColorToId(const Vector3 &charColor);

    Vector3 GetSelectionColor(GameObject *go) const;

public:
    SelectionFramebuffer(int width, int height);
    virtual ~SelectionFramebuffer();

    void RenderSelectionBuffer(const Scene *scene);
    void ProcessSelection();

    Material* GetSelectionMaterial() const;
    bool IsPassing() const;
};

#endif // SELECTIONFRAMEBUFFER_H
