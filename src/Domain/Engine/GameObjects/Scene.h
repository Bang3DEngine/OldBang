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
class GraphicPipeline;
class SelectionFramebuffer;
class Scene : public GameObject
{
protected:
    GraphicPipeline *m_gPipeline = nullptr;
    GameObject *m_defaultCamera = nullptr;

    //Framebuffer for positions, normals, uvs and diffuse
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

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
    virtual void PostReadXMLInfo(const XMLNode *xmlInfo) override;

    GraphicPipeline* GetGraphicPipeline() const;

    friend class Screen;
    friend class Toolbar;
    friend class GameObject;
    friend class Application;
    friend class SceneManager;
    friend class GraphicPipeline;
    friend class SelectionFramebuffer;
};

#endif // SCENE_H
