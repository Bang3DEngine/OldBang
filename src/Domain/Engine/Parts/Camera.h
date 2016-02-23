#ifndef CAMERA_H
#define CAMERA_H

#include "Bang.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

#include "Rect.h"
#include "Part.h"
#include "Transform.h"

class Canvas;
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

    float fovDegrees;
    float zNear;
    float zFar;

    mutable float aspectRatio;

    ProjectionMode projMode;

    bool autoUpdateAspectRatio;

public:
    Camera();

    void GetViewMatrix(glm::mat4 &view) const;
    void GetProjectionMatrix(glm::mat4 &proj) const;


    void SetOrthoRect(const Rect& rect);

    void SetFovDegrees(float fovDegrees);
    void SetZNear(float zNear);
    void SetZFar(float zFar);

    void SetProjectionMode(ProjectionMode projMode);

    void SetAspectRatio(float aspectRatio);

    void SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio);


    float GetFovDegrees() const;
    float GetAspectRatio() const;
    float GetZNear() const;
    float GetZFar() const;
    ProjectionMode GetProjectionMode() const;
    bool  GetAutoUpdateAspectRatio() const;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);

    virtual const std::string ToString() const override;
};

#endif // CAMERA_H
