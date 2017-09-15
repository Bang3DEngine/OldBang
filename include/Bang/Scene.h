#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/GameObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Gizmos;

class Scene : public GameObject
{
    SERIALIZABLE(Scene)

public:
    Scene();
    virtual ~Scene();

    void SetCamera(Camera *cam);
    void SetFirstFoundCameraOrDefaultOne();

    void Destroy(GameObject *gameObject);
    void DestroyImmediate(GameObject *gameObject);

    static Scene *GetActiveScene();
    virtual Camera *GetCamera() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;
    virtual void PostImportXML(const XMLNode &xmlInfo) override;

protected:
    ::std::queue<GameObject*> m_gameObjectsToBeDestroyed;
    GameObject *m_defaultCamera = nullptr;
    Camera *p_camera = nullptr;
    Gizmos *m_gizmos = nullptr;

    // GameObject
    virtual void RenderGizmos() override;

    void DestroyQueuedGameObjects();

    Gizmos *GetGizmos() const;

    friend class Gizmos;
    friend class Window;
    friend class Application;
    friend class SceneManager;
    friend class GPPass_G_Gizmos;
    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // SCENE_H
