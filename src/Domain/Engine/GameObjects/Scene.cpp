#include "Scene.h"
#include "GameObject.h"
#include "Canvas.h"

#include "DirectionalLight.h"

Scene::Scene() : GameObject("Scene")
{
    m_gbuffer = new GBuffer(Canvas::GetWidth(), Canvas::GetHeight());
    m_materialAfterLighting = AssetsManager::GetAsset<Material>("Assets/Engine/Materials/PR_AfterLighting.bmat");
}

void Scene::_OnStart()
{
    GameObject::_OnStart();

    if (!IsEditorGameObject())
    {
        std::list<Camera*> cameras = GetComponentsInChildren<Camera>();
        Logger_Log("Cameras: " << cameras);
        if (!cameras.empty())
        {
            Camera *cam = cameras.front();
            SetCamera(cam);
            Logger_Log("Found camera: " << cam);
        }
        else // Create default camera
        {
            Logger_Log("Creating default camera");
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

void Scene::_OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
}

Scene::~Scene()
{
    this->_OnDestroy();
    delete m_materialAfterLighting;
    delete m_defaultCamera;
    delete m_gbuffer;
}

void Scene::_OnRender()
{
    Camera *cam = m_cameraGameObject->GetComponent<Camera>();
    if (cam  && cam->GetAutoUpdateAspectRatio())
    {
        cam->SetAspectRatio( m_canvas->GetAspectRatio() );
    }

    m_gbuffer->Bind();
    m_gbuffer->ClearBuffersAndBackground(glm::vec4(0.9f,0.9f,0.9f,1));
    m_gbuffer->SetAllDrawBuffers();

    //From 0 to 9
    for (m_currentRenderLayer = 0; m_currentRenderLayer <= 9; ++m_currentRenderLayer)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        PROPAGATE_EVENT(_OnPreRender, m_children);
        PROPAGATE_EVENT(_OnRender, m_children);
    }

    // Apply lights to gbuffer
    std::list<Light*> childrenLights = GetComponentsInChildren<Light>();
    for (Light *light : childrenLights)
    {
        if (CAN_USE_COMPONENT(light))
        {
            light->ApplyLight(m_gbuffer);
        }
    }

    m_gbuffer->RenderPassWithMaterial(m_materialAfterLighting);

    m_gbuffer->UnBind();
    m_gbuffer->RenderToScreen();
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

Scene *Scene::GetCurrentScene()
{
    return Canvas::GetInstance()->GetCurrentScene();
}

Camera *Scene::GetCamera() const
{
    if (!m_cameraGameObject)
    {
        return nullptr;
    }

    return m_cameraGameObject->GetComponent<Camera>();
}

const Canvas *Scene::GetCanvas() const
{
    return m_canvas;
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
        std::string camId = xmlInfo->GetString("Camera");
        if (camId.length() > 0)
        {
            const Camera *cam = dynamic_cast<const Camera*>(XMLParser::GetPointerFromId(camId));
            if (cam)
            {
                SetCamera(cam);
            }
        }
    }
}




