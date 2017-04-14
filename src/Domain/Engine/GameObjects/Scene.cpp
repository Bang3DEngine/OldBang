#include "Bang/Scene.h"

#ifdef BANG_EDITOR
#include "Bang/EditorScene.h"
#endif

#include "Bang/Debug.h"
#include "Bang/Screen.h"
#include "Bang/Camera.h"
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
    #ifndef BANG_EDITOR
    if (!BehaviourManager::GetBehavioursMergedLibrary())
    {
        BehaviourManager::PrepareBehavioursLibrary(true); // Load precompiled lib
    }
    #endif
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
const String Scene::GetFileExtensionStatic()
{
    return "bscene";
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
        if (!cam->gameObject->HasHideFlag(HideFlags::HideInHierarchy))
        {
            SetCamera(cam);
            cameraFound = true;
            break;
        }
    }

    if (!cameraFound) // Create default camera
    {
        Debug_Warn("No camera was found. Creating default camera...");
        m_defaultCamera = new GameObject("DefaultCamera");
        m_defaultCamera->transform->SetPosition(Vector3(90));
        m_defaultCamera->transform->LookAt(Vector3::Zero);
        m_defaultCamera->SetParent(this);
        m_defaultCamera->AddHideFlag(HideFlags::HideAndDontSave);

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

Scene *Scene::GetDefaultScene()
{
    #ifdef BANG_EDITOR
    Scene *scene = new EditorScene();
    #else
    Scene *scene = new Scene();
    #endif

    GameObject *cube = MeshFactory::GetCubeGameObject();
    cube->SetParent(scene);

    const float c_dist = 5.0f;
    GameObject *dirLight = new GameObject("Directional Light");
    dirLight->AddComponent<DirectionalLight>();
    dirLight->transform->SetPosition(Vector3(-1, 1, -1) * c_dist);
    dirLight->transform->LookAt(cube);
    dirLight->SetParent(scene);

    GameObject *camera = new GameObject("Camera");
    Camera *camComp = camera->AddComponent<Camera>();
    camera->transform->SetPosition(Vector3(1, 1, -1) * c_dist);
    camera->transform->LookAt(cube);
    camera->AddComponent<AudioListener>();
    camComp->SetClearColor(Color::LightBlue);
    camera->SetParent(scene);

    return scene;
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
    if (HasHideFlag(HideFlags::DontSerialize))
    {
        xmlInfo = nullptr;
        return;
    }

    GameObject::Write(xmlInfo);
    xmlInfo->SetTagName("Scene");

    Camera *cam = GetCamera();
    xmlInfo->SetString("id", cam->GetInstanceId(), {XMLProperty::Hidden});
}

void Scene::PostRead(const XMLNode &xmlInfo)
{
    GameObject::PostRead(xmlInfo);

    String camId = xmlInfo.GetString("Camera");
    if (!camId.Empty())
    {
        const SerializableObject *f = XMLParser::GetPointerFromId(camId);
        const Camera *cam = Object::ConstCast<Camera>(f);
        if (cam)
        {
            SetCamera(cam);
        }
    }
}

