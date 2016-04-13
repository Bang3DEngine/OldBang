#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <cmath>

#include "Input.h"
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

    float mouseRotBoost = 500.0f;
    float mouseWheelBoost = 100.0f;

public:
    EditorCamera();
    virtual ~EditorCamera();

    void OnUpdate() override;
};

#endif // EDITORCAMERA_H
