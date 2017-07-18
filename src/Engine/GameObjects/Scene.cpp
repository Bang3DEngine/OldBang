#include "Bang/Scene.h"

#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/G_Screen.h"
#include "Bang/Material.h"
#include "Bang/XMLParser.h"
#include "Bang/Transform.h"
#include "Bang/Behaviour.h"
#include "Bang/GameObject.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/AudioListener.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/BehaviourManager.h"
#include "Bang/DirectionalLight.h"

Scene::Scene() : GameObject("Scene")
{
}

void Scene::_OnStart()
{
    GameObject::_OnStart();
}

void Scene::_OnUpdate()
{
    GameObject::_OnUpdate();
}

void Scene::_OnResize(int newWidth, int newHeight)
{
    List<RectTransform*> rectTransforms =
            GetComponentsInChildren<RectTransform>();
    for (RectTransform *rt : rectTransforms)
    {
        rt->OnParentSizeChanged();
    }
}

Scene::~Scene()
{
    delete m_defaultCamera;
}

void Scene::SetCamera(const Camera *cam)
{
    if (!cam)
    {
        m_cameraGameObject = nullptr;
        SetCamera(m_defaultCamera->GetComponent<Camera>());
    }
    else
    {
        m_cameraGameObject = cam->gameObject;
    }
}

void Scene::SetFirstFoundCameraOrDefaultOne()
{
    List<Camera*> cameras = GetComponentsInChildren<Camera>();
    bool cameraFound = false;
    for (Camera *cam : cameras)
    {
        SetCamera(cam);
        cameraFound = true;
        break;
    }

    if (!cameraFound) // Create default camera
    {
        Debug_Warn("No camera was found. Creating default camera...");
        m_defaultCamera = new GameObject("DefaultCamera");
        m_defaultCamera->transform->SetPosition(Vector3(90));
        m_defaultCamera->transform->LookAt(Vector3::Zero);
        m_defaultCamera->SetParent(this);

        Camera *cam = m_defaultCamera->AddComponent<Camera>();
        cam->SetFovDegrees(60.0f); cam->SetZNear(0.1f);
        cam->SetZFar(99999.0f);
        SetCamera(cam);
    }
}

Scene *Scene::GetActiveScene()
{
    return SceneManager::GetActiveScene();
}

Camera *Scene::GetCamera() const
{
    if (!m_cameraGameObject)
    {
        return nullptr;
    }
    return m_cameraGameObject->GetComponent<Camera>();
}

void Scene::Destroy(GameObject *gameObject)
{
    m_gameObjectsToBeDestroyed.push(gameObject);
}

void Scene::DestroyImmediate(GameObject *gameObject)
{
    gameObject->_OnDestroy();
    delete gameObject;
}

void Scene::DestroyQueuedGameObjects()
{
    while (!m_gameObjectsToBeDestroyed.empty())
    {
        GameObject *go = m_gameObjectsToBeDestroyed.front();
        DestroyImmediate(go);
        m_gameObjectsToBeDestroyed.pop();
    }
}

void Scene::Read(const XMLNode &xmlInfo)
{
    GameObject::Read(xmlInfo);
}

void Scene::Write(XMLNode *xmlInfo) const
{
    GameObject::Write(xmlInfo);
    xmlInfo->SetTagName("Scene");

    Camera *cam = GetCamera();
    xmlInfo->SetString("id", cam->GetInstanceId());
}

void Scene::PostRead(const XMLNode &xmlInfo)
{
    GameObject::PostRead(xmlInfo);
}
