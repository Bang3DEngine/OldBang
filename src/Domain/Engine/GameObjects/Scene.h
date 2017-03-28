#ifndef SCENE_H
#define SCENE_H

#include "Bang/List.h"

#include "Bang/Color.h"
#include "Bang/GameObject.h"

class Camera;
class GraphicPipeline;
class SelectionFramebuffer;
class Scene : public GameObject
{
    OBJECT(Scene)
    ICLONEABLE(Scene)

public:
    const static String GetFileExtensionStatic ();

    Scene();
    virtual ~Scene();

    void SetCamera (const Camera *cam);
    void SetFirstFoundCameraOrDefaultOne();

    static Scene *GetActiveScene();
    static Scene *GetDefaultScene();

    virtual Camera *GetCamera() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
    virtual void PostRead(const XMLNode &xmlInfo) override;

protected:
    GameObject *m_defaultCamera = nullptr;

    //Framebuffer for positions, normals, uvs and diffuse
    GameObject *m_cameraGameObject = nullptr;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnResize (int newWidth, int newHeight);

    friend class Screen;
    friend class Toolbar;
    friend class GameObject;
    friend class Application;
    friend class SceneManager;
    friend class GraphicPipeline;
    friend class SelectionFramebuffer;
};

#endif // SCENE_H
