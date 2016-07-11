#include "Scene.h"
#include "GameObject.h"
#include "Canvas.h"

Scene::Scene() : GameObject("Scene")
{
    m_isScene = true;
    gbuffer = new GBuffer(Canvas::GetWidth(),
                          Canvas::GetHeight());
}

void Scene::_OnResize(int newWidth, int newHeight)
{
    gbuffer->Resize(newWidth, newHeight);
}

Scene::~Scene()
{
    this->_OnDestroy();
    delete gbuffer;
}

void Scene::_OnRender()
{
    if(cameraGameObject )
    {
        Camera *cam = cameraGameObject->GetComponent<Camera>();
        if(cam  && cam->GetAutoUpdateAspectRatio())
        {
            cam->SetAspectRatio( canvas->GetAspectRatio() );
        }
    }

    gbuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //From 0 to 9
    for(currentRenderLayer = 0; currentRenderLayer <= 9; ++currentRenderLayer)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        PROPAGATE_EVENT(_OnPreRender, m_children);
        PROPAGATE_EVENT(_OnRender, m_children);
    }

    gbuffer->UnBind();
    gbuffer->RenderToScreen();
}

void Scene::SetCamera(const Camera *cam)
{
    if(!cam) this->cameraGameObject = nullptr;
    else this->cameraGameObject = cam->gameObject;
}

Camera *Scene::GetCamera() const
{
    if(!cameraGameObject) return nullptr;
    return cameraGameObject->GetComponent<Camera>();
}

const Canvas *Scene::GetCanvas() const
{
    return canvas;
}

#ifdef BANG_EDITOR
void Scene::Write(std::ostream &f) const
{
    f << "<Scene>" << std::endl;
    f << ((void*)this) << std::endl;   //internal file id
    f << this->GetName() << std::endl; //scene name

    f << "<children>" << std::endl;
    for(GameObject *e : m_children)
    {
        e->Write(f);
    }
    f << "</children>" << std::endl;

    //Not used ftm
    //f << "<cameraGameObject>" << std::endl;
    //f << "</cameraGameObject>" << std::endl;

    f << "</Scene>" << std::endl;
}
#endif
