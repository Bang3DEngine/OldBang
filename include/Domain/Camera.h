#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "Rect.h"
#include "Part.h"
#include "Transform.h"

class Camera : public Part
{
public:
    enum ProjectionMode
    {
        Orthographic,
        Perspective
    };

private:

    Rect orthoRect;

    float fov;
    float zNear;
    float zFar;

    float aspectRatio;

    ProjectionMode projMode;

    bool autoUpdateAspectRatio;

public:
    Camera();


    void GetViewMatrix(glm::mat4 &view) const;
    void GetProjectionMatrix(glm::mat4 &proj) const;


    void SetOrthoRect(const Rect& rect);

    void SetFov(float fov);
    void SetZNear(float zNear);
    void SetZFar(float zFar);

    void SetAspectRatio(float aspectRatio);

    void SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio);


    float GetFov() const;
    float GetAspectRatio() const;
    float GetZNear() const;
    float GetZFar() const;
    bool  GetAutoUpdateAspectRatio() const;
};

#endif // CAMERA_H
