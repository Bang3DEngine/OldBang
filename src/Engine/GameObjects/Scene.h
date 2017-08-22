#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/GameObject.h"

FORWARD class Camera;
FORWARD class Gizmos;

class Scene : public GameObject
{
    SERIALIZABLE_OBJECT(Scene)

public:
    Scene();
    virtual ~Scene();

    void SetCamera(Camera *cam);
    void SetFirstFoundCameraOrDefaultOne();

    void Destroy(GameObject *gameObject);
    void DestroyImmediate(GameObject *gameObject);

    static Scene *GetActiveScene();
    virtual Camera *GetCamera() const;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
    virtual void PostRead(const XMLNode &xmlInfo) override;

protected:
    std::queue<GameObject*> m_gameObjectsToBeDestroyed;
    List<GameObject*> m_hiddenGameObjects;
    GameObject *m_defaultCamera = nullptr;
    Camera *p_camera = nullptr;
    Gizmos *m_gizmos = nullptr;

    // GameObject
    virtual void Start () override;
    virtual void Update () override;
    virtual void RenderGizmos() override;
    virtual void _OnResize (int newWidth, int newHeight);

    void AddHiddenChild(GameObject *go);
    void DestroyQueuedGameObjects();

    Gizmos *GetGizmos() const;

    friend class Screen;
    friend class Gizmos;
    friend class Application;
    friend class SceneManager;
    friend class GPPass_G_Gizmos;
    friend class GraphicPipeline;
};

#endif // SCENE_H
