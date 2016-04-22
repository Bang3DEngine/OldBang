#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <cmath>

#include "Bang.h"

#include "Input.h"
#include "Canvas.h"
#include "Camera.h"
#include "EditorEntity.h"

//ENTITY Camera that you can control in the editor
class EditorCamera : public EditorEntity
{
private:
    Camera *cam = nullptr;

    float maxMoveSpeed = 1.0f;
    float minMoveSpeed = 0.1f;
    float moveAccel = 0.1f;
    float moveSpeed = 0.0f;

    float mouseCamPlaneMoveBoost = 60.0f; //Movement with middle button
    float mouseRotBoost = 5.0f;
    float mouseWheelBoost = 50.0f;

    //Only used if using cam orbitting behaviour
    glm::vec2 mouseRotationRads = glm::vec2(0.0f);

public:
    EditorCamera();
    virtual ~EditorCamera();

    void OnUpdate() override;
};

#endif // EDITORCAMERA_H
