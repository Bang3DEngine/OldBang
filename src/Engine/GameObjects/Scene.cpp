#include "Bang/Scene.h"

#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"

Scene::Scene() : GameObject("Scene")
{
    m_gizmos = new Gizmos();
    AddHiddenChild(m_gizmos);
    AddComponent<Transform>();
}

Scene::~Scene()
{
}

void Scene::Start()
{
    GameObject::Start();
    PROPAGATE_EVENT(Start(), m_hiddenGameObjects);
}

void Scene::Update()
{
    GameObject::Update();
    PROPAGATE_EVENT(Update(), m_hiddenGameObjects);
}

void Scene::RenderGizmos()
{
    GameObject::RenderGizmos();
    PROPAGATE_EVENT(RenderGizmos(), m_hiddenGameObjects);
}

void Scene::_OnResize(int newWidth, int newHeight)
{
    ParentSizeChanged();
}

void Scene::AddHiddenChild(GameObject *go)
{
    go->SetParent(this);
    if (!m_hiddenGameObjects.Contains(go)) { m_hiddenGameObjects.PushBack(go); }
}

Gizmos *Scene::GetGizmos() const { return m_gizmos; }

void Scene::SetCamera(Camera *cam)
{
    if (!cam)
    {
        p_camera = nullptr;
        SetCamera(m_defaultCamera->GetComponent<Camera>());
    }
    else
    {
        p_camera = cam;
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
        m_defaultCamera->AddComponent<Transform>();
        m_defaultCamera->transform->SetPosition(Vector3(90));
        m_defaultCamera->transform->LookAt(Vector3::Zero);
        AddHiddenChild(m_defaultCamera);

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
    return p_camera;
}

void Scene::Destroy(GameObject *gameObject)
{
    m_gameObjectsToBeDestroyed.push(gameObject);
}

void Scene::DestroyImmediate(GameObject *gameObject)
{
    gameObject->Destroy();
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
    xmlInfo->Set("id", cam->GetInstanceId());
}

void Scene::PostRead(const XMLNode &xmlInfo)
{
    GameObject::PostRead(xmlInfo);
}
