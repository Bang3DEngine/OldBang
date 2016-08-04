#ifndef SCENE_H
#define SCENE_H

#include "Bang.h"

#include <list>
#include <string>

#include "Color.h"
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

    //Framebuffer for positions, normals, uvs and diffuse
    GBuffer *m_gbuffer = nullptr;
    const Canvas *m_canvas = nullptr; //set by canvas when added to it
    GameObject *m_cameraGameObject = nullptr;

    virtual void _OnStart () override;
    virtual void _OnResize (int newWidth, int newHeight);

public:
    const static std::string GetFileExtension () { return "bscene"; }

    Scene();
    virtual ~Scene();

    virtual void _OnRender () override;

    void SetCamera (const Camera *cam);

    static Scene *GetCurrentScene();
    static Camera* GetCamera ();
    Camera* GetCurrentCamera () const;
    const Canvas* GetCanvas () const;

    bool IsScene() const override;

    virtual void DebugDrawLine (const Vector3 &origin,
                                const Vector3 &destiny,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f,
                                bool depthTest = true) {}

    virtual void DebugDrawScreenLine (
                                const Vector2 &origin,
                                const Vector2 &destiny,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f,
                                bool depthTest = true) {}

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
    virtual void PostReadXMLInfo(const XMLNode *xmlInfo) override;
};

#endif // SCENE_H
