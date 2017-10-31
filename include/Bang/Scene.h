#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/GameObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Gizmos;
FORWARD class UILayoutManager;

class Scene : public GameObject
{
    SERIALIZABLE(Scene)

public:
    Scene();
    virtual ~Scene();

    virtual void Update() override;
    virtual void OnResize(int newWidth, int newHeight);

    void SetCamera(Camera *cam);
    void SetFirstFoundCameraOrDefaultOne();

    void Destroy(GameObject *gameObject);
    void DestroyImmediate(GameObject *gameObject);

    static Scene *GetActiveScene();
    UILayoutManager* GetUILayoutManager() const;
    virtual Camera *GetCamera() const;

    void InvalidateCanvas();

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;
    virtual void PostImportXML(const XMLNode &xmlInfo) override;

protected:
    UILayoutManager *m_uiLayoutManager = nullptr;
    std::queue<GameObject*> m_gameObjectsToBeDestroyed;
    GameObject *m_defaultCamera = nullptr;
    Camera *p_camera = nullptr;
    Gizmos *m_gizmos = nullptr;

    // GameObject
    virtual void RenderGizmos() override;

    void DestroyQueuedGameObjects();

    Gizmos *GetGizmos() const;

    friend class Gizmos;
    friend class Window;
    friend class GEngine;
    friend class Application;
    friend class SceneManager;
};

NAMESPACE_BANG_END

#endif // SCENE_H
