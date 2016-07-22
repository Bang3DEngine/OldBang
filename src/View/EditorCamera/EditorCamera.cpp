#include "EditorCamera.h"

#include "Canvas.h"

EditorCamera::EditorCamera() : EditorGameObject("EditorCamera")
{
    m_yawNode = new EditorGameObject("EditorYawNode");
    m_yawNode->SetParent(this);

    m_cam = m_yawNode->AddComponent<Camera>();
    m_cam->SetAutoUpdateAspectRatio(true);
    m_cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    m_t = transform;
    m_t->SetPosition(Vector3(10.0f, 10.0f, 10.0f));
    m_t->LookAt(Vector3(0));
    UpdateRotationVariables();

    m_camt = m_yawNode->transform;
    m_cam->SetZFar(999.9f);
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::AdjustSpeeds()
{
    Canvas *c = Canvas::GetInstance();
    int cw = c->GetWidth();
    int ch = c->GetHeight();

    m_mouseRotDegreesPerPixel.x = 180.0f / cw;
    m_mouseRotDegreesPerPixel.y = 180.0f / ch;

    m_mousePanPerPixel = glm::vec2(0.1f, 0.1f);
    if (m_currentFocus)
    {
        Transform *ft = m_currentFocus->transform;
        if (ft )
        {
            Vector3 focusPoint = ft->GetPosition();
            float d = Vector3::Distance(focusPoint, m_t->GetPosition());
            float ar = c->GetAspectRatio();
            float halfFov = glm::radians(m_cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = glm::tan(halfFov) * d;
            m_mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / ch;
            m_mousePanPerPixel.x = m_mousePanPerPixel.y * ch * ar / cw;
        }
    }
}

void EditorCamera::UpdateRotationVariables()
{
    m_mouseRotDegreesAccum = glm::vec2(0.0f);
    m_startingRotation = m_t->GetLocalRotation();
}

void EditorCamera::HandleWheelZoom(Vector3 *moveStep, bool *hasMoved)
{
    if (!Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mouseWheel = Input::GetMouseWheel();
        if (mouseWheel != 0.0f)
        {
            *moveStep -= m_mouseZoomPerDeltaWheel * mouseWheel * m_camt->GetForward();
            *hasMoved  = true;
        }
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if (Input::GetMouseButton(Input::MouseButton::MRight))
    {
        glm::vec2 delta = -Input::GetMouseDelta() * m_mouseRotDegreesPerPixel;
        m_mouseRotDegreesAccum += delta;

        m_t->SetLocalRotation(m_startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(glm::radians(m_mouseRotDegreesAccum.x),
                                                Vector3::up);
        m_t->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(glm::radians(m_mouseRotDegreesAccum.y),
                                                m_camt->GetRight());
        m_t->Rotate(rotY);

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
        glm::vec2 delta = -Input::GetMouseDelta() * m_mousePanPerPixel;
        delta.y *= -1.0f;

        m_t->Translate(m_camt->GetRight() * delta.x + m_camt->GetUp() * delta.y);

        Canvas::SetCursor(Qt::SizeAllCursor);
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

            Vector3 thisPos = m_t->GetPosition();
            Vector3 focusPos = focusBSphere.GetCenter();
            Vector3 focusDir = (focusPos - thisPos).Normalized();

            //LookAt Rotation
            if (thisPos != focusPos)
            {
                Quaternion origin = m_t->GetRotation();
                Quaternion dest = Quaternion::LookDirection(focusDir, Vector3::up);
                Quaternion final = Quaternion::Slerp( origin, dest,
                            Time::GetDeltaTime() * m_lookAtRotSpeed);

                m_t->SetLocalRotation(final);
            }

            //LookAt Move
            float minDist = 0.0f;
            float radius = focusBSphere.GetRadius();
            if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
            {
                float fov = glm::radians(cam->GetFovDegrees() / 2.0f);
                minDist = radius / std::tan(fov);
            }

            minDist = std::max(minDist, 0.5f); //In case boundingBox is empty
            Vector3 dest = focusPos - (focusDir * minDist);
            float t = Time::GetDeltaTime() * m_lookAtMoveSpeed;
            m_t->SetPosition( Vector3::Lerp(thisPos, dest, t) );
        }
    }
}

void EditorCamera::OnUpdate()
{
    AdjustSpeeds();

    Vector3 moveStep(0.0f);
    bool hasMoved = false;
    bool unwrapMouse = true;

    HandleKeyMovement(&moveStep, &hasMoved); //WASD

    if (!HandleMouseRotation(&hasMoved, &unwrapMouse)) //Mouse rot with right click
    {
        HandleMousePanning(&hasMoved, &unwrapMouse); //Mouse move with mid click
    }

    HandleWheelZoom(&moveStep, &hasMoved);

    HandleLookAtFocus();
    if (m_doingLookAt)
    {   //Update all needed variables in case we are doing a lookAt.
        UpdateRotationVariables();
    }

    m_keysMoveSpeed += m_keysMoveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    m_keysMoveSpeed = glm::clamp(m_keysMoveSpeed, m_minMoveSpeed, m_maxMoveSpeed);

    if (unwrapMouse)
    {
        Canvas::SetCursor( Qt::ArrowCursor ); //cursor visible
        Input::SetMouseWrapping(false);
    }

    if (!hasMoved )
    {
        m_keysMoveSpeed = 0.0f; //reset speed
    }
    else
    {
        m_doingLookAt = false; //No more lookAt
        m_t->SetPosition(m_t->GetPosition() + moveStep); //Move camera the amount gathered
    }
}

Camera *EditorCamera::GetCamera()
{
    return m_yawNode->GetComponent<Camera>();
}

#ifdef BANG_EDITOR

void EditorCamera::OnTreeHierarchyGameObjectsSelected
    (std::list<GameObject *> &selectedGameObjects)
{
    if (selectedGameObjects.size() != 1) return;

    GameObject *selected = selectedGameObjects.front();
    m_currentFocus = selected;
    m_doingLookAt = false;
}


void EditorCamera::OnTreeHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    NONULL(selected);
    m_currentFocus = selected;
    m_doingLookAt = true;
}
#endif

