#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/GameObject.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Gizmos;

class Scene : public GameObject,
              public IDestroyListener
{
    GAMEOBJECT(Scene);

public:
    virtual void Update() override;
    virtual void OnResize(int newWidth, int newHeight);

    void SetCamera(Camera *cam);
    void SetFirstFoundCamera();

    static Scene *GetActiveScene();
    virtual Camera *GetCamera() const;

    void InvalidateCanvas();

    // IDestroyListener
    void OnDestroyed(Object *object) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;
    virtual void PostImportXML(const XMLNode &xmlInfo) override;

protected:
    Camera *p_camera = nullptr;
    Gizmos *m_gizmos = nullptr;

    Scene();
    virtual ~Scene();

    Gizmos *GetGizmos() const;

    // GameObject
    virtual void RenderGizmos() override;

    friend class Gizmos;
    friend class Window;
    friend class GEngine;
    friend class Application;
    friend class SceneManager;
};

NAMESPACE_BANG_END

#endif // SCENE_H
