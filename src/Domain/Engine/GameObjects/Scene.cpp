#include "Scene.h"
#include "GameObject.h"
#include "Canvas.h"

Scene::Scene() : GameObject("Scene")
{
    isScene = true;
    gbuffer = new GBuffer(Canvas::GetWidth(), Canvas::GetHeight());
}

void Scene::_OnResize(int newWidth, int newHeight)
{
    gbuffer->Resize(newWidth, newHeight);
}

Scene::~Scene()
{
    this->_OnDestroy();
}

void Scene::_OnRender()
{
    gbuffer->Bind();
    //All the mesh renderers now will render stuff into the geometryFramebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(cameraGameObject != nullptr)
    {
        Camera *cam = cameraGameObject->GetComponent<Camera>();
        if(cam != nullptr && cam->GetAutoUpdateAspectRatio())
        {
            cam->SetAspectRatio( canvas->GetAspectRatio() );
        }
    }

    PROPAGATE_EVENT(_OnRender, children);
    gbuffer->UnBind();

    gbuffer->RenderToScreen();
}

void Scene::SetCameraChild(const std::string &cameraChildName)
{
    GameObject *cameraGameObject = GetChild(cameraChildName);
    if(cameraGameObject != nullptr)
    {
        if(cameraGameObject->HasComponent<Camera>())
        {
            this->cameraGameObject = cameraGameObject;
        }
        else
        {
            Logger_Error("Can't set " << cameraGameObject <<
                         " as camera because it does not have a Camera component.");
        }
    }
}

Camera *Scene::GetCamera() const
{
    if(cameraGameObject == nullptr) return nullptr;
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
    for(GameObject *e : children)
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
