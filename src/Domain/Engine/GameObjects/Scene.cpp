#include "Scene.h"

#include "Screen.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"
#include "DirectionalLight.h"

Scene::Scene() : GameObject("Scene")
{
    m_gPipeline = new GraphicPipeline();
}

void Scene::_OnStart()
{
    GameObject::_OnStart();

    if (!IsEditorGameObject())
    {
        List<Camera*> cameras = GetComponentsInChildren<Camera>();
        Debug_Log("Cameras: " << cameras);
        if (!cameras.Empty())
        {
            Camera *cam = cameras.Front();
            SetCamera(cam);
            Debug_Log("Found camera: " << cam);
        }
        else // Create default camera
        {
            Debug_Log("Creating default camera");
            GameObject *m_defaultCamera = new GameObject("DefaultCamera");
            m_defaultCamera->transform->SetPosition(Vector3(90));
            m_defaultCamera->transform->LookAt(Vector3::zero);
            m_defaultCamera->SetParent(this);

            Camera *cam = m_defaultCamera->AddComponent<Camera>();
            cam->SetFovDegrees(60.0f); cam->SetZNear(0.1f);
            cam->SetZFar(99999.0f);
            SetCamera(cam);
        }
    }
}

void Scene::_OnUpdate()
{
    GameObject::_OnUpdate();

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
    m_gPipeline->OnResize(newWidth, newHeight);
}

Scene::~Scene()
{
    _OnDestroy();
    delete m_gPipeline;
    delete m_defaultCamera;
}

void Scene::_OnRender()
{
    NONULL(m_cameraGameObject);
    Camera *cam = m_cameraGameObject->GetComponent<Camera>();
    if (cam  && cam->GetAutoUpdateAspectRatio())
    {
        cam->SetAspectRatio( m_screen->GetAspectRatio() );
    }

    m_gPipeline->RenderScene(this);
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

Scene *Scene::GetActiveScene()
{
    return SceneManager::GetActiveScene();
}

Camera *Scene::GetCamera()
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene->GetCurrentCamera();
}

Camera *Scene::GetCurrentCamera() const
{
    if (!m_cameraGameObject)
    {
        return nullptr;
    }
    return m_cameraGameObject->GetComponent<Camera>();
}

const Screen *Scene::GetScreen() const
{
    return m_screen;
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

GraphicPipeline *Scene::GetGraphicPipeline() const
{
    return m_gPipeline;
}


