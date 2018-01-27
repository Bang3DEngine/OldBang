#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/GameObject.h"
#include "Bang/BehaviourManager.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Gizmos;
FORWARD class DebugRenderer;

class Scene : public GameObject,
              public IDestroyListener
{
    GAMEOBJECT(Scene);

public:
    virtual void OnPreStart() override;
    virtual void Update() override;
    virtual void Render(RenderPass rp, bool renderChildren = true) override;
    virtual void OnResize(int newWidth, int newHeight);

    void SetCamera(Camera *cam);
    void SetFirstFoundCamera();

    virtual Camera *GetCamera() const;
    BehaviourManager *GetBehaviourManager() const;

    void InvalidateCanvas();

    ObjectManager *GetLocalObjectManager() const;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;
    virtual void PostImportXML(const XMLNode &xmlInfo) override;

protected:
    Camera *p_camera = nullptr;
    Gizmos *m_gizmos = nullptr;
    DebugRenderer *p_debugRenderer = nullptr;
    ObjectManager *m_localObjectManager = nullptr;
    BehaviourManager *m_behaviourManager = nullptr;

    Scene();
    virtual ~Scene();

    Gizmos *GetGizmos() const;
    DebugRenderer *GetDebugRenderer() const;

private:
    virtual BehaviourManager* CreateBehaviourManager() const;

    friend class Gizmos;
    friend class Window;
    friend class GEngine;
    friend class Application;
    friend class SceneManager;
    friend class DebugRenderer;
};

NAMESPACE_BANG_END

#endif // SCENE_H
