#ifndef SCENE_H
#define SCENE_H

#include "Bang.h"

#include "List.h"


#include "Color.h"
#include "Debug.h"
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
friend class Toolbar;
friend class GameObject;
friend class Application;
friend class SceneManager;
friend class SelectionFramebuffer;

protected:

    GameObject *m_defaultCamera = nullptr;
    Material *m_materialBeforeLighting = nullptr;
    Material *m_materialAfterLighting   = nullptr;

    //Framebuffer for positions, normals, uvs and diffuse
    GBuffer *m_gbuffer = nullptr;
    const Screen *m_screen = nullptr; //set by screen when added to it
    GameObject *m_cameraGameObject = nullptr;

    // Indicates whether we are currently rendering opaque or transparent Renderers
    // Renderer's will use it to determine if they can render themselves or not
    bool m_opaquePass = false;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnResize (int newWidth, int newHeight);

    /**
     * @brief Renders the opaque Renderers, using Deferred rendering.
     */
    void RenderOpaque();

    /**
     * @brief Renders the transparent Renderers, using Forward rendering.
     * It renders them from furthest to closest.
     */
    void RenderTransparent();

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

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
    virtual void PostReadXMLInfo(const XMLNode *xmlInfo) override;

    bool IsInOpaquePass() const;
};

#endif // SCENE_H
