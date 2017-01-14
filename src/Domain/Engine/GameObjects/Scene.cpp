#include "Scene.h"

#include "Debug.h"
#include "Screen.h"
#include "Camera.h"
#include "Material.h"
#include "XMLParser.h"
#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"
#include "DirectionalLight.h"

Scene::Scene() : GameObject("Scene")
{
}

void Scene::_OnStart()
{
    GameObject::_OnStart();

    SetFirstFoundCameraOrDefaultOne();
}

void Scene::_OnUpdate()
{
    GameObject::_OnUpdate();

    Camera *cam = m_cameraGameObject->GetComponent<Camera>();
    if (cam  && cam->GetAutoUpdateAspectRatio())
    {
        cam->SetAspectRatio( Screen::GetInstance()->GetAspectRatio() );
    }

    // Start non-started GameObjects
    for (GameObject *child : m_children)
    {
        if (!child->IsStarted())
        {
            child->_OnStart();
        }
    }
}

void Scene::_OnResize(int newWidth, int newHeight)
{
}

const String Scene::GetFileExtensionStatic()
{
    return "bscene";
}

Scene::~Scene()
{
    _OnDestroy();
    delete m_defaultCamera;
}

void Scene::CloneInto(ICloneable *clone) const
{
    GameObject::CloneInto(clone);
}

ICloneable *Scene::Clone() const
{
    Scene *scene = new Scene();
    CloneInto(scene);
    return scene;
}

void Scene::SetCamera(const Camera *cam)
{
    if (!cam)
    {
        this->m_cameraGameObject = nullptr;
        SetCamera(m_defaultCamera->GetComponent<Camera>());
    }
    else
    {
        this->m_cameraGameObject = cam->gameObject;
    }
}

void Scene::SetFirstFoundCameraOrDefaultOne()
{
    List<Camera*> cameras = GetComponentsInChildren<Camera>();
    bool cameraFound = false;
    for (Camera *cam : cameras)
    {
        if (!cam->gameObject->IsEditorGameObject())
        {
            SetCamera(cam);
            cameraFound = true;
            break;
        }
    }

    if (!cameraFound) // Create default camera
    {
        Debug_Warn("No camera was found. Creating default camera...");
        GameObject *m_defaultCamera = new GameObject("DefaultCamera");
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

Camera *Scene::GetCamera()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene ? scene->GetCurrentCamera() : nullptr;
}

Camera *Scene::GetCurrentCamera() const
{
    if (!m_cameraGameObject)
    {
        return nullptr;
    }
    return m_cameraGameObject->GetComponent<Camera>();
}

bool Scene::IsScene() const
{
    return true;
}

void Scene::ReadXMLInfo(const XMLNode *xmlInfo)
{
    GameObject::ReadXMLInfo(xmlInfo);
}

void Scene::FillXMLInfo(XMLNode *xmlInfo) const
{
    GameObject::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Scene");

    Camera *cam = GetCamera();
    xmlInfo->SetPointer("id", cam, {XMLProperty::Hidden});
}

void Scene::PostReadXMLInfo(const XMLNode *xmlInfo)
{
    GameObject::PostReadXMLInfo(xmlInfo);

    if (!IsEditorGameObject()) // In EditorScene we'll use the EditorCamera
    {
        String camId = xmlInfo->GetString("Camera");
        if (!camId.Empty())
        {
            const Camera *cam = dynamic_cast<const Camera*>(XMLParser::GetPointerFromId(camId));
            if (cam)
            {
                SetCamera(cam);
            }
        }
    }
}

