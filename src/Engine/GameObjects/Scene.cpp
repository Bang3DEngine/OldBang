#include "Bang/Scene.h"

#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/XMLNode.h"
#include "Bang/GEngine.h"
#include "Bang/UICanvas.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
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

void Scene::Update()
{
    GameObject::Update();
}

void Scene::OnResize(int newWidth, int newHeight)
{
    InvalidateCanvas();
}

void Scene::RenderGizmos()
{
    GameObject::RenderGizmos();
    GetGizmos()->m_gizmosGo->RenderGizmos();
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
        m_defaultCamera = GameObjectFactory::CreateGameObjectNamed("DefaultCamera");
        m_defaultCamera->GetTransform()->SetPosition(Vector3(5));
        m_defaultCamera->GetTransform()->LookAt(Vector3::Zero);
        m_defaultCamera->SetParent(this);
        m_defaultCamera->GetHideFlags().SetOn(HideFlag::DontSave);

        sceneCamera = m_defaultCamera->AddComponent<Camera>();
        sceneCamera->SetFovDegrees(60.0f);
        sceneCamera->SetZNear(0.1f);
        sceneCamera->SetZFar(99999.0f);
    }

    SetCamera(sceneCamera);
}

void Scene::InvalidateCanvas()
{
    List<UICanvas*> canvases = GetComponentsInChildren<UICanvas>(true);
    for (UICanvas *canvas : canvases)
    {
        canvas->GetGameObject()->GetComponent<RectTransform>()->Invalidate();
    }

    List<RectTransform*> rts = GetComponentsInChildren<RectTransform>(true);
    for (RectTransform *rt : rts)
    {
        rt->Invalidate();
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
