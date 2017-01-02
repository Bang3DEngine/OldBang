#include "EditorCamera.h"

#include "Debug.h"
#include "Time.h"
#include "Input.h"
#include "Scene.h"
#include "Screen.h"
#include "Camera.h"
#include "Screen.h"
#include "Transform.h"
#include "SceneManager.h"

EditorCamera::EditorCamera() : EditorGameObject("BANG_EditorCamera")
{
    m_yawNode = new EditorGameObject("BANG_EditorYawNode");
    m_yawNode->SetParent(this);

    m_cam = m_yawNode->AddComponent<Camera>();
    m_cam->SetAutoUpdateAspectRatio(true);
    m_cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    m_camt = m_yawNode->transform;
    m_cam->SetZNear(0.05f);
    m_cam->SetZFar(9999.9f);
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
            float halfFov = glm::radians(m_cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = glm::tan(halfFov) * d;
            m_mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / ch;
            m_mousePanPerPixel.x = m_mousePanPerPixel.y * ch * ar / cw;
        }
    }

    m_mousePanPerPixel.x = glm::max(m_mousePanPerPixel.x, 0.05f);
    m_mousePanPerPixel.y = glm::max(m_mousePanPerPixel.y, 0.05f);
}

void EditorCamera::UpdateRotationVariables()
{
    m_mouseRotDegreesAccum = Vector2(0.0f);
    m_startingRotation = transform->GetLocalRotation();
}

void EditorCamera::HandleWheelZoom(Vector3 *moveStep, bool *hasMoved)
{
    if (!Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mouseWheel = Input::GetMouseWheel();
        if (mouseWheel != 0.0f)
        {
            *moveStep += m_mouseZoomPerDeltaWheel * mouseWheel * m_camt->GetForward();
            *hasMoved  = true;
        }
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if (Input::GetMouseButton(Input::MouseButton::MRight))
    {
        Vector2 delta = -Input::GetMouseDelta() * m_mouseRotDegreesPerPixel;
        m_mouseRotDegreesAccum += delta;

        transform->SetLocalRotation(m_startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(glm::radians(m_mouseRotDegreesAccum.x),
                                                Vector3::Up);
        transform->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(glm::radians(m_mouseRotDegreesAccum.y),
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

        Screen::SetCursor(Qt::SizeAllCursor);
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
        if (m_currentFocus->HasComponent<Transform>())
        {
            Sphere focusBSphere = m_currentFocus->GetBoundingSphere();

            Vector3 thisPos = transform->GetPosition();
            Vector3 focusPos = focusBSphere.GetCenter();
            Vector3 focusDir = (focusPos - thisPos).Normalized();

            //LookAt Rotation
            if (thisPos != focusPos)
            {
                Quaternion origin = transform->GetRotation();
                Quaternion dest = Quaternion::LookDirection(focusDir, Vector3::Up);
                Quaternion final = Quaternion::Slerp( origin, dest,
                            Time::GetDeltaTime() * m_lookAtRotSpeed);

                transform->SetLocalRotation(final);
            }

            //LookAt Move
            float stopDist = 0.0f;
            float radius = focusBSphere.GetRadius();
            if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
            {
                float fov = glm::radians(cam->GetFovDegrees() / 2.0f);
                stopDist = radius / std::tan(fov) * 1.5f;
            }

            stopDist = std::max(stopDist, 1.0f); //In case boundingBox is empty
            Vector3 dest = focusPos - (focusDir * stopDist);
            float t = Time::GetDeltaTime() * m_lookAtMoveSpeed;
            transform->SetPosition( Vector3::Lerp(thisPos, dest, t) );

            if ( Vector3::Distance(dest, thisPos) < 0.05f)
            {
                m_doingLookAt = false;
            }
        }
    }
}

void EditorCamera::OnStart()
{
    EditorGameObject::OnStart();

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
    EditorGameObject::OnUpdate();
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
        m_keysMoveSpeed = glm::clamp(m_keysMoveSpeed, m_minMoveSpeed, m_maxMoveSpeed);
        if (!hasMoved)
        {
            m_keysMoveSpeed = 0.0f; //reset speed
        }

        transform->Translate(moveStep);
    }
    else
    {
        HandleLookAtFocus(); // Modifies m_doingLookAt

        //Update all needed variables in case we are doing a lookAt.
        UpdateRotationVariables();
    }

    if (unwrapMouse)
    {
        Screen::SetCursor( Qt::ArrowCursor ); //cursor visible
        Input::SetMouseWrapping(false);
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

#ifdef BANG_EDITOR

void EditorCamera::OnHierarchyGameObjectsSelected
    (List<GameObject *> &selectedGameObjects)
{
    if (selectedGameObjects.Size() != 1) return;

    GameObject *selected = selectedGameObjects.Front();
    m_currentFocus = selected;
    m_doingLookAt = false;
}


void EditorCamera::OnHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    EXISTS(selected);
    m_currentFocus = selected;
    m_doingLookAt = true;
}
#endif

