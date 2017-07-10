#ifndef SCENE_H
#define SCENE_H

#include <queue>

#include "Bang/List.h"
#include "Bang/Color.h"
#include "Bang/GameObject.h"

class Camera;
class GraphicPipeline;
class SelectionFramebuffer;
class Scene : public GameObject
{
    OBJECT(Scene)

public:
    const static String GetFileExtensionStatic ();

    Scene();
    virtual ~Scene();

    void SetCamera (const Camera *cam);
    void SetFirstFoundCameraOrDefaultOne();

    static Scene *GetActiveScene();

    void Destroy(GameObject *gameObject);
    void DestroyImmediate(GameObject *gameObject);

    virtual Camera *GetCamera() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
    virtual void PostRead(const XMLNode &xmlInfo) override;

    #ifdef BANG_EDITOR
    virtual void OnHierarchyGameObjectsSelected(
            const List<GameObject*> &selectedEntities) override;
    #endif

protected:
    std::queue<GameObject*> m_gameObjectsToBeDestroyed;
    GameObject *m_defaultCamera = nullptr;

    //G_Framebuffer for positions, normals, uvs and diffuse
    GameObject *m_cameraGameObject = nullptr;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnResize (int newWidth, int newHeight);

    void DestroyQueuedGameObjects();

    friend class Screen;
    friend class Toolbar;
    friend class Application;
    friend class SceneManager;
    friend class GraphicPipeline;
    friend class G_SelectionFramebuffer;
};

#endif // SCENE_H
