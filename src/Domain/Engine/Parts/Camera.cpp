#include "Camera.h"
#include "Canvas.h"
#include "StageReader.h"

Camera::Camera() : orthoRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f)),
                   fovDegrees(60.0f),
                   zNear(0.1f),
                   zFar(100.0f),
                   aspectRatio(1.0f),
                   projMode(ProjectionMode::Perspective),
                   autoUpdateAspectRatio(true)
{
}

void Camera::GetViewMatrix(glm::mat4 &view) const
{
    Transform *t = GetOwner()->GetPart<Transform>();
    if(t != nullptr)
    {
        t->GetMatrix(view);
        view = glm::inverse(view);
    }
    else
    {
        Logger_Warning(GetOwner() << " has a Camera but does not have a transform. " <<
                       "View matrix will be the identity matrix.");

        view = glm::mat4(1.0f);
    }
}

void Camera::GetProjectionMatrix(glm::mat4 &proj) const
{
    if(projMode == ProjectionMode::Perspective)
    {
        if(autoUpdateAspectRatio)
        {
            aspectRatio = Canvas::GetAspectRatio();
        }

        proj = glm::perspective(glm::radians(fovDegrees), aspectRatio, zNear, zFar);
    }
    else //Ortho
    {
        proj = glm::ortho(orthoRect.left, orthoRect.right,
                          orthoRect.bottom, orthoRect.top,
                          zNear, zFar);
    }
}

void Camera::SetOrthoRect(const Rect &rect)
{
    orthoRect = rect;
}







void Camera::SetFovDegrees(float fovDegrees)
{
    this->fovDegrees = fovDegrees;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
}

void Camera::SetZNear(float zNear)
{
    this->zNear = zNear;
}

void Camera::SetZFar(float zFar)
{
    this->zFar = zFar;
}

void Camera::SetProjectionMode(Camera::ProjectionMode projMode)
{
    this->projMode = projMode;
}

void Camera::SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio)
{
    this->autoUpdateAspectRatio = autoUpdateAspectRatio;
}






float Camera::GetFovDegrees() const
{
    return fovDegrees;
}

float Camera::GetAspectRatio() const
{
    return aspectRatio;
}

float Camera::GetZNear() const
{
    return zNear;
}

float Camera::GetZFar() const
{
    return zFar;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
    return projMode;
}

bool Camera::GetAutoUpdateAspectRatio() const
{
    return autoUpdateAspectRatio;
}

void Camera::Write(std::ostream &f) const
{

}

void Camera::Read(std::istream &f)
{
    StageReader::RegisterNextPointerId(f, this);
    SetFovDegrees( FileReader::ReadFloat(f) );
    SetZNear( FileReader::ReadFloat(f) );
    SetZFar( FileReader::ReadFloat(f) );
    SetProjectionMode( FileReader::ReadString(f) == "Perspective" ?
                                            Camera::ProjectionMode::Perspective :
                                            Camera::ProjectionMode::Orthographic);
    SetOrthoRect( FileReader::ReadRect(f) );
    FileReader::ReadNextLine(f); //Consume close tag
}

const std::string Camera::ToString() const
{
    return "Camera";
}
