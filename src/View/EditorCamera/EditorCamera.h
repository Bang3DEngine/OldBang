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
    Transform *t = nullptr; //YawNode transform
    Camera *cam = nullptr;
    Transform *camt = nullptr; //YawNode transform
    GameObject *yawNode = nullptr;

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

    /**
     * @brief Handles camera 'zoom' with mouse wheel
     * If it has moved, hasMoved = true.
     */
    void HandleWheelZoom(Vector3 *moveStep, bool *hasMoved);

    /**
     * @brief Handles camera rotation with right mouse click
     * If it has rotated, hasMoved = true.
     * If it has rotated, unlockMouse = false
     * @return Returns if the cam has rotated or not
     */
    bool HandleMouseRotation(bool *hasMoved, bool *unlockMouse);

    /**
     * @brief Handles camera panning (moving in the plane
     * perpendicular to cam's forward) with middle mouse click
     * If it has moved, hasMoved = true.
     * If it has moved, unlockMouse = false
     */
    void HandleMousePanning(bool *hasMoved, bool *unlockMouse);

    /**
     * @brief Handles key movement with WASD
     */
    void HandleKeyMovement(Vector3 *moveStep, bool *hasMoved);

    /**
     * @brief Handles LookAt behaviour when focusing a GameObject.
     */
    void HandleLookAtFocus();

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
