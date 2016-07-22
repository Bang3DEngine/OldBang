#include "Scene.h"
#include "GameObject.h"
#include "Canvas.h"

#include "DirectionalLight.h"

Scene::Scene() : GameObject("Scene")
{
    GameObject *test = new GameObject("LightTest");
    test->AddComponent<DirectionalLight>();
    test->SetParent(this);

    m_gbuffer = new GBuffer(Canvas::GetWidth(), Canvas::GetHeight());
}

void Scene::_OnResize(int newWidth, int newHeight)
{
    m_gbuffer->Resize(newWidth, newHeight);
}

Scene::~Scene()
{
    this->_OnDestroy();
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

void Scene::Write(std::ostream &f) const
{
    f << "<Scene>" << std::endl;
    f << ((void*)this) << std::endl;   //internal file id
    f << this->GetName() << std::endl; //scene name

    f << "<children>" << std::endl;
    for (GameObject *e : m_children)
    {
        e->Write(f);
    }
    f << "</children>" << std::endl;

    //Not used ftm
    //f << "<cameraGameObject>" << std::endl;
    //f << "</cameraGameObject>" << std::endl;

    f << "</Scene>" << std::endl;
}
