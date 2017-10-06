#include "Bang/Scene.h"

#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/GEngine.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/UILayoutManager.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

Scene::Scene() : GameObject("Scene")
{
    m_gizmos = new Gizmos();
    m_uiLayoutManager = new UILayoutManager();
}

Scene::~Scene()
{
}

#include "Bang/Input.h"
void Scene::Update()
{
    GameObject::Update();
}

void Scene::RenderGizmos()
{
    GameObject::RenderGizmos();
    m_gizmos->m_gizmosGo->SetParent(this);
    GetGizmos()->m_gizmosGo->RenderGizmos();
    m_gizmos->m_gizmosGo->SetParent(nullptr);
}

Gizmos *Scene::GetGizmos() const { return m_gizmos; }

void Scene::SetCamera(Camera *cam)
{
    if (p_camera)
    {
        p_camera->SetGameObjectToRender(nullptr);
        p_camera = nullptr;
    }

    p_camera = cam;
    if (p_camera)
    {
        p_camera->SetGameObjectToRender(this);
    }
}

void Scene::SetFirstFoundCameraOrDefaultOne()
{
    Camera *sceneCamera = nullptr;
    List<Camera*> cameras = GetComponentsInChildren<Camera>();
    if (!cameras.IsEmpty()) { sceneCamera = cameras.Front(); }
    else
    {
        // Create default camera
        m_defaultCamera = GameObjectFactory::CreateGameObject(true);
        m_defaultCamera->SetName("DefaultCamera");
        m_defaultCamera->transform->SetPosition(Vector3(5));
        m_defaultCamera->transform->LookAt(Vector3::Zero);
        m_defaultCamera->SetParent(this);
        m_defaultCamera->GetHideFlags().SetOn(HideFlag::DontSave);

        Camera *cam = m_defaultCamera->AddComponent<Camera>();
        cam->SetFovDegrees(60.0f); cam->SetZNear(0.1f);
        cam->SetZFar(99999.0f);
    }

    SetCamera(sceneCamera);
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

Scene *Scene::GetActiveScene() { return SceneManager::GetActiveScene(); }

UILayoutManager *Scene::GetUILayoutManager() const { return m_uiLayoutManager; }
Camera *Scene::GetCamera() const { return p_camera; }

void Scene::ImportXML(const XMLNode &xmlInfo)
{
    GameObject::ImportXML(xmlInfo);
    SetFirstFoundCameraOrDefaultOne();
}

void Scene::ExportXML(XMLNode *xmlInfo) const
{
    GameObject::ExportXML(xmlInfo);
    xmlInfo->SetTagName("Scene");
}

void Scene::PostImportXML(const XMLNode &xmlInfo)
{
    GameObject::PostImportXML(xmlInfo);
}
