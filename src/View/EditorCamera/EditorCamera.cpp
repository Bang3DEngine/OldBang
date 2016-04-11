#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
    this->SetName("EditorCamera");

    cam = new Camera();
    this->AddPart(cam);

    cam->SetAutoUpdateAspectRatio(true);
    cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    Transform *t = new Transform();
    this->AddPart(t);

}

EditorCamera::~EditorCamera()
{
}
