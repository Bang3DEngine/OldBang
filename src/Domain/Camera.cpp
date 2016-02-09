#include "Camera.h"

Camera::Camera() : orthoRect(Rect(-1.0f, 1.0f, -1.0f, 1.0f)),
                   fov(60.0f),
                   zNear(0.1f),
                   zFar(100.0f),
                   aspectRatio(1.0f),
                   projMode(ProjectionMode::Perspective),
                   autoUpdateAspectRatio(true)
{
}

void Camera::GetViewMatrix(glm::mat4 &view) const
{

}

void Camera::GetProjectionMatrix(glm::mat4 &proj) const
{
    if(projMode == ProjectionMode::Perspective)
    {
        proj = glm::perspective(fov, aspectRatio, zNear, zFar);
    }
    else //Ortho
    {
        proj = glm::ortho(orthoRect.left, orthoRect.right,
                          orthoRect.bottom, orthoRect.top);
    }
}

void Camera::SetOrthoRect(const Rect &rect)
{
    orthoRect = rect;
}







void Camera::SetFov(float fov)
{
    this->fov = fov;
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

void Camera::SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio)
{
    this->autoUpdateAspectRatio = autoUpdateAspectRatio;
}






float Camera::GetFov() const
{
    return fov;
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

bool Camera::GetAutoUpdateAspectRatio() const
{
    return autoUpdateAspectRatio;
}
