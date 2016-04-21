#include "Stage.h"
#include "Entity.h"
#include "Canvas.h"

Stage::Stage()
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
    if(cameraEntity != nullptr)
    {
        Camera *cam = cameraEntity->GetPart<Camera>();
        if(cam != nullptr && cam->GetAutoUpdateAspectRatio())
        {
            cam->SetAspectRatio( canvas->GetAspectRatio() );
        }
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

#ifdef BANG_EDITOR
void Stage::Write(std::ostream &f) const
{
    f << "<Stage>" << std::endl;
    f << ((void*)this) << std::endl;   //internal file id
    f << this->GetName() << std::endl; //stage name

    //Children come now
    f << "<children>" << std::endl;
    for(Entity *e : children)
    {
        e->Write(f);
    }
    f << "</children>" << std::endl;

    //Not used ftm
    //f << "<cameraEntity>" << std::endl;
    //f << "</cameraEntity>" << std::endl;

    f << "</Stage>" << std::endl;
}
#endif
