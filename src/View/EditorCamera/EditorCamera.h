#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <cmath>

#include "Bang.h"

#include "Vector3.h"

#include "Input.h"
#include "Canvas.h"
#include "Camera.h"
#include "EditorGameObject.h"
#include "IWindowEventManagerListener.h"


//Pre-built GameObject, with a Camera that you can control in the editor
//This camera has a parent "Pitch-Node", which rotates in y
// and a child "Yaw-Node", which rotates in x.
// The Yaw-Node contains the camera Component itself

class EditorCamera : public EditorGameObject
{
private:
    GameObject *yawNode = nullptr;
    Camera *cam = nullptr;
    Transform *camt = nullptr; //YawNode transform

    GameObject *currentLookAtFocus = nullptr;
    float lookAtRotSpeed = 5.0f;
    float lookAtMoveSpeed = 4.0f;

    float maxMoveSpeed = 1.0f;
    float minMoveSpeed = 0.1f;
    float moveAccel = 0.1f;
    float moveSpeed = 0.0f;

    const float mouseCamPlaneMoveBoost = 150.0f; //Movement with middle button
    float mouseRotBoost = 15.0f;
    float mouseWheelBoost = 24.0f;

    //Only used if using cam orbitting behaviour
    glm::vec2 mouseRotationDegrees = glm::vec2(0.0f);
    float mouseRotX, mouseRotY;

public:
    EditorCamera();
    virtual ~EditorCamera();

    void OnUpdate() override;

    /**
     * Gets the Camera Component inside the "Yaw-Node".
     **/
    Camera *GetCamera();

#ifdef BANG_EDITOR
    void OnTreeHierarchyGameObjectDoubleClicked(GameObject *selected) override;
#endif
};

#endif // EDITORCAMERA_H
