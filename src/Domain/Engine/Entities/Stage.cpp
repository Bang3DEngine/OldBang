#include "Stage.h"
#include "Entity.h"
#include "Canvas.h"

Stage::Stage() : cameraEntity(nullptr)
{
    isStage = true;
    gbuffer = new GBuffer(Canvas::GetWidth(), Canvas::GetHeight());
}

void Stage::_OnResize(int newWidth, int newHeight)
{
    gbuffer->Resize(newWidth, newHeight);
}

Stage::~Stage()
{
    this->_OnDestroy();
}

void Stage::_OnRender()
{
    gbuffer->Bind();
    //All the mesh renderers now will render stuff into the geometryFramebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(cameraEntity != nullptr && cameraEntity->GetPart<Camera>()->GetAutoUpdateAspectRatio())
    {
        cameraEntity->GetPart<Camera>()->SetAspectRatio( canvas->GetAspectRatio() );
    }

    PROPAGATE_EVENT(_OnRender, children);
    gbuffer->UnBind();

    gbuffer->RenderToScreen();
}

void Stage::SetCameraChild(const std::string &cameraChildName)
{
    Entity *cameraEntity = GetChild(cameraChildName);
    if(cameraEntity != nullptr)
    {
        if(cameraEntity->HasPart<Camera>())
        {
            this->cameraEntity = cameraEntity;
        }
        else
        {
            Logger_Error("Can't set " << cameraEntity <<
                         " as camera because it does not have a Camera part.");
        }
    }
}

Camera *Stage::GetCamera() const
{
    if(cameraEntity == nullptr) return nullptr;
    return cameraEntity->GetPart<Camera>();
}

const Canvas *Stage::GetCanvas() const
{
    return canvas;
}

