#ifndef SCENE_H
#define SCENE_H

#include "Bang.h"

#include <list>


#include "Color.h"
#include "Logger.h"
#include "Camera.h"
#include "Screen.h"
#include "GBuffer.h"
#include "Material.h"
#include "GameObject.h"

class Screen;
class SelectionFramebuffer;
class Scene : public GameObject
{

friend class Screen;
friend class GameObject;
friend class Application;
friend class SceneManager;
friend class SelectionFramebuffer;

protected:

    GameObject *m_defaultCamera = nullptr;
    Material *m_materialAfterLighting = nullptr;

    //Framebuffer for positions, normals, uvs and diffuse
    GBuffer *m_gbuffer = nullptr;
    const Screen *m_screen = nullptr; //set by screen when added to it
    GameObject *m_cameraGameObject = nullptr;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnResize (int newWidth, int newHeight);

public:
    const static String GetFileExtension () { return "bscene"; }

    Scene();
    virtual ~Scene();

    virtual void _OnRender () override;

    void SetCamera (const Camera *cam);

    static Scene *GetActiveScene();
    static Camera* GetCamera ();
    Camera* GetCurrentCamera () const;
    const Screen* GetScreen () const;

    bool IsScene() const override;

    #ifdef BANG_EDITOR
    virtual void DebugDrawLine (const Vector3 &origin,
                                const Vector3 &destiny,
                                const Color &color,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f,
                                bool depthTest = true) {}

    virtual void DebugDrawScreenLine (
                                const Vector2 &origin,
                                const Vector2 &destiny,
                                const Color &color,
                                float lineWidth = 1.0f,
                                float secsTime = 1.0f) {}
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
    virtual void PostReadXMLInfo(const XMLNode *xmlInfo) override;
};

#endif // SCENE_H
