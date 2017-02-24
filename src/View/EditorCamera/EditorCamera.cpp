#include "EditorCamera.h"

#include "Math.h"
#include "Time.h"
#include "Debug.h"
#include "Input.h"
#include "Scene.h"
#include "Screen.h"
#include "Cursor.h"
#include "Camera.h"
#include "Screen.h"
#include "Transform.h"
#include "SceneManager.h"

float EditorCamera::s_initialFovDegrees = 60.0f;
float EditorCamera::s_initialZNear      = 0.05f;
float EditorCamera::s_initialZFar       = 9999.9f;

EditorCamera::EditorCamera() : GameObject("BANG_EditorCamera")
{
    m_yawNode = new GameObject("BANG_EditorYawNode");
    m_yawNode->AddHideFlag(HideFlags::HideAndDontSave);
    m_yawNode->SetParent(this);

    m_cam = m_yawNode->AddComponent<Camera>();
    m_cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    m_camt = m_yawNode->transform;
    m_cam->SetZNear(s_initialZNear);
    m_cam->SetZFar(s_initialZFar);
    m_cam->SetFovDegrees(EditorCamera::s_initialFovDegrees);

    AddHideFlag(HideFlags::HideAndDontSave);
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::AdjustSpeeds()
{
    Screen *c = Screen::GetInstance();
    int cw = c->GetWidth();
    int ch = c->GetHeight();

    m_mouseRotDegreesPerPixel.x = 180.0f / cw;
    m_mouseRotDegreesPerPixel.y = 180.0f / ch;

    m_mousePanPerPixel = Vector2(0.1f, 0.1f);
    if (m_currentFocus)
    {
        Transform *ft = m_currentFocus->transform;
        if (ft)
        {
            Vector3 focusPoint = ft->GetPosition();
            float d = Vector3::Distance(focusPoint, transform->GetPosition());
            float ar = c->GetAspectRatio();
            float halfFov = Math::Deg2Rad(m_cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = Math::Tan(halfFov) * d;
            m_mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / ch;
            m_mousePanPerPixel.x = m_mousePanPerPixel.y * ch * ar / cw;
        }
    }

    m_mousePanPerPixel.x = Math::Max(m_mousePanPerPixel.x, 0.05f);
    m_mousePanPerPixel.y = Math::Max(m_mousePanPerPixel.y, 0.05f);
}

void EditorCamera::UpdateRotationVariables()
{
    m_mouseRotDegreesAccum = Vector2(0.0f);
    m_startingRotation = transform->GetLocalRotation();
}

void EditorCamera::HandleWheelZoom(Vector3 *moveStep, bool *hasMoved)
{
    // Update zoom value
    float mouseWheel = Input::GetMouseWheel() * m_mouseZoomPerDeltaWheel;
    m_zoomValue = mouseWheel + (m_zoomValue * 0.1f); // Lerp

    // Apply zoom
    if (m_zoomValue != 0.0f)
    {
        if (m_cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
            *moveStep += m_zoomValue * m_camt->GetForward();
            *hasMoved  = true;
        }
        //Debug_Log(mouseWheel);
        m_orthoHeight -= 2.75f * mouseWheel; // Magic number here :)
        m_cam->SetOrthoHeight(m_orthoHeight);
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if (Input::GetMouseButton(Input::MouseButton::MRight))
    {
        Vector2 delta = -Input::GetMouseDelta() * m_mouseRotDegreesPerPixel;
        m_mouseRotDegreesAccum += delta;

        transform->SetLocalRotation(m_startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(Math::Deg2Rad(m_mouseRotDegreesAccum.x),
                                                Vector3::Up);
        transform->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(Math::Deg2Rad(m_mouseRotDegreesAccum.y),
                                                m_camt->GetRight());
        transform->Rotate(rotY);

        Input::SetMouseWrapping(true);
        *hasMoved  = true;
        *unwrapMouse = false;
        return true;
    }
    return false;
}

void EditorCamera::HandleMousePanning(bool *hasMoved, bool *unwrapMouse)
{
    if (Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        Vector2 delta = -Input::GetMouseDelta() * m_mousePanPerPixel;
        delta.y *= -1.0f;

        transform->Translate(m_camt->GetRight() * delta.x + m_camt->GetUp() * delta.y);

        Cursor::SetIcon(Cursor::CursorIcon::MoveArrows);
        Input::SetMouseWrapping(true);
        *hasMoved  = true;
        *unwrapMouse = false;
    }
}

void EditorCamera::HandleKeyMovement(Vector3 *moveStep, bool *hasMoved)
{
    Vector3 m(0);
    if (Input::GetKey(Input::Key::W))
    {
        m += m_keysMoveSpeed * Time::GetDeltaTime() * m_camt->GetForward();
    }
    else if (Input::GetKey(Input::Key::S))
    {
        m -= m_keysMoveSpeed * Time::GetDeltaTime() * m_camt->GetForward();
    }

    if (Input::GetKey(Input::Key::A))
    {
        m -= m_keysMoveSpeed * Time::GetDeltaTime() * m_camt->GetRight();
    }
    else if (Input::GetKey(Input::Key::D))
    {
        m += m_keysMoveSpeed * Time::GetDeltaTime() * m_camt->GetRight();
    }

    *moveStep += m;
    *hasMoved = *hasMoved || (m.Length() != 0);
}

void EditorCamera::HandleLookAtFocus()
{
    if (m_doingLookAt)
    {
        Camera *cam = GetCamera();
        Sphere focusBSphere = m_currentFocus->GetBoundingSphere();

        Vector3 thisPos = transform->GetPosition();
        Vector3 focusPos = focusBSphere.GetCenter();
        Vector3 focusDir = (focusPos - thisPos).Normalized();

        //LookAt Rotation
        Quaternion originRot = transform->GetRotation();
        Quaternion destRot = Quaternion::LookDirection(focusDir, Vector3::Up);
        Quaternion final = Quaternion::Slerp(
                    originRot, destRot, Time::GetDeltaTime() * m_lookAtRotSpeed);

        transform->SetLocalRotation(final);

        //LookAt Move
        float stopDist = 0.0f;
        float radius = focusBSphere.GetRadius();
        if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
        {
            float fov = Math::Deg2Rad(cam->GetFovDegrees() / 2.0f);
            stopDist = radius / std::tan(fov) * 1.5f;
        }
        stopDist = std::max(stopDist, 1.0f); //In case boundingBox is empty
        Vector3 dest = focusPos - (focusDir * stopDist);
        float t = Time::GetDeltaTime() * m_lookAtMoveSpeed;
        transform->SetPosition( Vector3::Lerp(thisPos, dest, t) );

        m_doingLookAt =
                Vector3::Distance(dest, thisPos) > 0.05f ||
                Vector3::Dot(transform->GetForward(), focusDir) < 0.99999f;
    }

}

void EditorCamera::OnStart()
{
    GameObject::OnStart();

    Sphere sceneSphere = SceneManager::GetActiveScene()->GetBoundingSphere();
    if (sceneSphere.GetRadius() > 0)
    {
        transform->SetPosition(Vector3(1.0f) * sceneSphere.GetRadius());
        transform->LookAt(sceneSphere.GetCenter());
    }
    else
    {
        transform->SetPosition(Vector3::One * 30.0f);
        transform->LookAt(Vector3::Zero);
    }

    UpdateRotationVariables();
}

void EditorCamera::OnUpdate()
{
    GameObject::OnUpdate();
    AdjustSpeeds();

    Vector3 moveStep(0.0f);
    bool hasMoved = false;
    bool unwrapMouse = true;

    if (!m_doingLookAt)
    {
        HandleKeyMovement(&moveStep, &hasMoved); //WASD

        if (!HandleMouseRotation(&hasMoved, &unwrapMouse)) //Mouse rot with right click
        {
            HandleMousePanning(&hasMoved, &unwrapMouse); //Mouse move with mid click
        }

        HandleWheelZoom(&moveStep, &hasMoved);

        m_keysMoveSpeed += m_keysMoveAccel; //TODO: must do this in FixedUpdate which does not exist yet
        m_keysMoveSpeed = Math::Clamp(m_keysMoveSpeed, m_minMoveSpeed, m_maxMoveSpeed);
        if (!hasMoved)
        {
            m_keysMoveSpeed = 0.0f; //reset speed
        }

        transform->Translate(moveStep);
    }
    else
    {
        UpdateRotationVariables();
        HandleLookAtFocus(); // Modifies m_doingLookAt
        UpdateRotationVariables();
    }

    if (unwrapMouse && Input::GetMouseWrapping())
    {
        Input::SetMouseWrapping(false);
        Cursor::SetIcon(Cursor::CursorIcon::Arrow);
    }
}


void EditorCamera::OnGameObjectDestroyed(GameObject *destroyed)
{
    if (m_currentFocus == destroyed)
    {
        m_currentFocus = nullptr;
        m_doingLookAt = false;
    }
}

void EditorCamera::AlignViewWithGameObject(GameObject *selected)
{
    m_currentFocus = nullptr;
    m_doingLookAt = false;
    m_camt->SetLocalRotation(Quaternion::Identity);
    transform->SetPosition(selected->transform->GetPosition());
    Vector3 up = Vector3::Up;
    transform->LookInDirection(selected->transform->GetForward(), up);
    UpdateRotationVariables();
}

Camera *EditorCamera::GetCamera()
{
    return m_yawNode->GetComponent<Camera>();
}

void EditorCamera::SwitchProjectionModeTo(bool mode3D)
{
    if (mode3D)
    {
        m_cam->SetProjectionMode(Camera::ProjectionMode::Perspective);
        m_cam->SetZNear(EditorCamera::s_initialZNear);
        m_cam->SetZFar(EditorCamera::s_initialZFar);
        m_cam->SetFovDegrees(EditorCamera::s_initialFovDegrees);
    }
    else
    {
        m_cam->SetProjectionMode(Camera::ProjectionMode::Orthographic);
        m_cam->SetOrthoHeight(m_orthoHeight);
        m_cam->SetZNear(-999999.9f);
        m_cam->SetZFar(999999.9f);
    }
}

#ifdef BANG_EDITOR

void EditorCamera::OnHierarchyGameObjectsSelected
    (List<GameObject *> &selectedGameObjects)
{
    /*
    if (selectedGameObjects.Size() != 1) return;
    GameObject *selected = selectedGameObjects.Front();
    m_currentFocus = selected;
    m_doingLookAt = false;
    */
}


void EditorCamera::OnHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    ASSERT(selected);
    StartLookAt(selected);
}

void EditorCamera::StartLookAt(GameObject *lookAtFocus)
{
    m_currentFocus = lookAtFocus;
    m_doingLookAt = true;
}
#endif

