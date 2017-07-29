#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/GameObject.h"

FORWARD class Camera;
FORWARD class Gizmos;

class Scene : public GameObject
{
    OBJECT(Scene)

public:
    Scene();
    virtual ~Scene();

    void SetCamera(Camera *cam);
    void SetFirstFoundCameraOrDefaultOne();

    static Scene *GetActiveScene();

    void Destroy(GameObject *gameObject);
    void DestroyImmediate(GameObject *gameObject);

    virtual Camera *GetCamera() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
    virtual void PostRead(const XMLNode &xmlInfo) override;

protected:
    std::queue<GameObject*> m_gameObjectsToBeDestroyed;
    List<GameObject*> m_hiddenGameObjects;
    GameObject *m_defaultCamera = nullptr;
    Camera *p_camera = nullptr;
    Gizmos *m_gizmos = nullptr;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnDrawGizmos() override;
    virtual void _OnResize (int newWidth, int newHeight);

    void AddHiddenChild(GameObject *go);

    Gizmos *GetGizmos() const;
    void DestroyQueuedGameObjects();

    friend class Screen;
    friend class Gizmos;
    friend class Application;
    friend class SceneManager;
    friend class GPPass_G_Gizmos;
    friend class GraphicPipeline;
};

#endif // SCENE_H
