#ifndef SCENE_H
#define SCENE_H

#include "Bang.h"

#include <list>
#include <string>

#include "Logger.h"
#include "Camera.h"
#include "Canvas.h"
#include "GBuffer.h"
#include "Material.h"
#include "GameObject.h"

class Canvas;
class SelectionFramebuffer;
class Scene : public GameObject
{

friend class Canvas;
friend class GameObject;
friend class SelectionFramebuffer;

protected:

    GameObject *m_defaultCamera = nullptr;
    Material *m_materialAfterLighting = nullptr;

    /**
     * @brief currentRenderLayer is a variable that indicates
     * the currentRenderLayer being used to render, so GameObjects
     * can decide wether to render or not.
     */
    int m_currentRenderLayer = 0;

    //Framebuffer for positions, normals, uvs and diffuse
    GBuffer *m_gbuffer = nullptr;
    const Canvas *m_canvas = nullptr; //set by canvas when added to it
    GameObject *m_cameraGameObject = nullptr;

    virtual void _OnResize (int newWidth, int newHeight);

public:
    const static std::string GetFileExtension () { return "bscene"; }

    Scene();
    virtual ~Scene();

    void _OnRender () override;

    void SetCamera (const Camera *cam);

    Camera* GetCamera () const;
    const Canvas* GetCanvas () const;

    bool IsScene() const override;

    virtual void DebugDrawLine (const Vector3 &origin,
                                const Vector3 &destiny,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f,
                                bool depthTest = true) {}

    virtual void DebugDrawScreenLine (
                                const glm::vec2 &origin,
                                const glm::vec2 &destiny,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f,
                                bool depthTest = true) {}

    virtual void ReadXMLNode(const XMLNode *xmlNode) override;
    virtual void GetXMLNode(XMLNode *xmlNode) const override;
};

#endif // SCENE_H
