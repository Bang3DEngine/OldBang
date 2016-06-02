#include "Scene.h"
#include "GameObject.h"
#include "Canvas.h"
#include "SelectionFramebuffer.h"

Scene::Scene() : GameObject("Scene")
{
    isScene = true;
    gbuffer = new GBuffer(Canvas::GetWidth(),
                          Canvas::GetHeight());
    selectionFramebuffer =
            new SelectionFramebuffer(Canvas::GetWidth(),
                                     Canvas::GetHeight());
}

void Scene::_OnResize(int newWidth, int newHeight)
{
    gbuffer->Resize(newWidth, newHeight);
    selectionFramebuffer->Resize(newWidth, newHeight);
}

Scene::~Scene()
{
    this->_OnDestroy();
    delete selectionFramebuffer;
    delete gbuffer;
}

void Scene::_OnRender(unsigned char _renderLayer)
{
    if(cameraGameObject != nullptr)
    {
        Camera *cam = cameraGameObject->GetComponent<Camera>();
        if(cam != nullptr && cam->GetAutoUpdateAspectRatio())
        {
            cam->SetAspectRatio( canvas->GetAspectRatio() );
        }
    }

    gbuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //From 0 to 9
    for(unsigned char renderLayer = 0; renderLayer <= 9; ++renderLayer)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        PROPAGATE_EVENT_WITH_RENDER_LAYER(renderLayer, _OnPreRender, children);
        PROPAGATE_EVENT_WITH_RENDER_LAYER(renderLayer, _OnRender, children);
    }

    gbuffer->UnBind();
    gbuffer->RenderToScreen();

#ifdef BANG_EDITOR
    selectionFramebuffer->Bind();
    selectionFramebuffer->RenderSelectionBuffer(this);
    selectionFramebuffer->ProcessSelection();
    selectionFramebuffer->UnBind();
#endif
}

void Scene::SetCamera(const Camera *cam)
{
    this->cameraGameObject = cam->GetOwner();
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
