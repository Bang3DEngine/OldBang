#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <cmath>

#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Quaternion.h"
#include "Bang/GameObject.h"

class Input;
class Camera;
class G_Screen;

/**
 * Pre-built GameObject, with a Camera that you can control in the editor
 * This camera has a parent "Pitch-Node", which rotates in y
 * and a child "Yaw-Node", which rotates in x.
 * The Yaw-Node contains the camera Component itself
**/
class EditorCamera : public GameObject
{
public:

    EditorCamera();
    virtual ~EditorCamera();

    void OnGameObjectDestroyed(GameObject *destroyed) override;
    void AlignViewWithGameObject(GameObject *selected);

    static float s_initialFovDegrees;
    static float s_initialZNear;
    static float s_initialZFar;

    void SwitchProjectionModeTo(bool mode3D);

    #ifdef BANG_EDITOR
    void OnHierarchyGameObjectsSelected
                (const List<GameObject*> &selectedGameObjects) override;
    void OnHierarchyGameObjectDoubleClicked(GameObject *selected) override;
    #endif

    void StartLookAt(GameObject *lookAtFocus);

    /**
     * Gets the Camera Component inside the "Yaw-Node".
     **/
    Camera *GetCamera();


protected:
    /**
     * @brief Adjust different camera parameters based
     * on the current camera position, focus, etc. For example,
     * it adjusts mousePanPerPixel. mouseRotDegreesPerPixel, etc.
     */
    void AdjustSpeeds();

    /**
     * @brief Called after every change on the camera rotation which is not
     * from user's input. This is to update all internal variables related with
     * rotation handling.
     */
    void UpdateRotationVariables();

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
    bool HandleMouseRotation(bool *hasMoved, bool *unwrapMouse);

    /**
     * @brief Handles camera panning (moving in the plane
     * perpendicular to cam's forward) with middle mouse click
     * If it has moved, hasMoved = true.
     * If it has moved, unlockMouse = false
     */
    void HandleMousePanning(bool *hasMoved, bool *unwrapMouse);

    /**
     * @brief Handles key movement with WASD
     */
    void HandleKeyMovement(Vector3 *moveStep, bool *hasMoved);

    /**
     * @brief Handles LookAt behaviour when focusing a GameObject.
     */
    void HandleLookAtFocus();

    void OnEditorStart() override;
    void OnEditorUpdate() override;

private:
    Camera *m_cam         = nullptr;
    Transform *m_camt     = nullptr; //YawNode transform
    GameObject *m_yawNode = nullptr;

    GameObject *m_currentFocus = nullptr;
    float m_lookAtRotSpeed     = 3.0f;
    float m_lookAtMoveSpeed    = 4.0f;

    /**
     * These 4 variables are related to the speed when moving
     * the camera with WASD
     */
    float m_maxMoveSpeed  = 1.0f;
    float m_minMoveSpeed  = 0.1f;
    float m_keysMoveAccel = 0.1f;
    float m_keysMoveSpeed = 1.0f;

    /**
     * @brief How many units in world space do we have to move,
     * for every panned pixel?
     */
    Vector2 m_mousePanPerPixel = Vector2(70.0f); //Movement with middle button

    /**
     * @brief startingRotation changes after every lookAt.
     * It indicates the initial offset to sum to the user's rotation
     * of mouse right click.
     */
    Quaternion m_startingRotation;                         //starting offset
    Vector2 m_mouseRotDegreesAccum = Vector2(0.0f);    //User input
    Vector2 m_mouseRotDegreesPerPixel = Vector2(0.0f); //Parameter

    /**
     * @brief How many units in world space do we have to move
     * in camera's forward direction, for every user wheel delta?
     */
    float m_mouseZoomPerDeltaWheel = 0.05f;
    float m_zoomValue = 0.0f;

    float m_orthoHeight = 30.0f;
};

#endif // EDITORCAMERA_H
