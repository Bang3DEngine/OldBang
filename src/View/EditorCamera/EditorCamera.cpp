#include "EditorCamera.h"

#include "Canvas.h"

EditorCamera::EditorCamera() : EditorGameObject("EditorCamera")
{
    p_yawNode = new EditorGameObject("EditorYawNode");
    AddChild(p_yawNode);

    p_cam = p_yawNode->AddComponent<Camera>();
    p_cam->SetAutoUpdateAspectRatio(true);
    p_cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    p_t = AddComponent<Transform>();
    p_t->SetPosition(Vector3(10.0f, 10.0f, 10.0f));
    p_t->LookAt(Vector3(0));
    UpdateRotationVariables();

    p_camt = p_yawNode->AddComponent<Transform>();
    p_cam->SetZFar(999.9f);
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
    if(p_currentFocus)
    {
        Transform *ft = p_currentFocus->GetComponent<Transform>();
        if(ft )
        {
            Vector3 focusPoint = ft->GetPosition();
            float d = Vector3::Distance(focusPoint, p_t->GetPosition());
            float ar = c->GetAspectRatio();
            float halfFov = glm::radians(p_cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = glm::tan(halfFov) * d;
            m_mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / ch;
            m_mousePanPerPixel.x = m_mousePanPerPixel.y * ch * ar / cw;
        }
    }
}

void EditorCamera::UpdateRotationVariables()
{
    m_mouseRotDegreesAccum = glm::vec2(0.0f);
    m_startingRotation = p_t->GetLocalRotation();
}

void EditorCamera::HandleWheelZoom(Vector3 *moveStep, bool *hasMoved)
{
    if(!Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mouseWheel = Input::GetMouseWheel();
        if(mouseWheel != 0.0f)
        {
            *moveStep -= m_mouseZoomPerDeltaWheel * mouseWheel * p_camt->GetForward();
            *hasMoved  = true;
        }
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        glm::vec2 delta = -Input::GetMouseDelta() * m_mouseRotDegreesPerPixel;
        m_mouseRotDegreesAccum += delta;

        p_t->SetLocalRotation(m_startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(glm::radians(m_mouseRotDegreesAccum.x),
                                                Vector3::up);
        p_t->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(glm::radians(m_mouseRotDegreesAccum.y),
                                                p_camt->GetRight());
        p_t->Rotate(rotY);

        Input::SetMouseWrapping(true);
        *hasMoved  = true;
        *unwrapMouse = false;
        return true;
    }
    return false;
}

void EditorCamera::HandleMousePanning(bool *hasMoved, bool *unwrapMouse)
{
    if(Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        glm::vec2 delta = -Input::GetMouseDelta() * m_mousePanPerPixel;
        delta.y *= -1.0f;

        p_t->Translate(p_camt->GetRight() * delta.x + p_camt->GetUp() * delta.y);

        Canvas::SetCursor(Qt::SizeAllCursor);
        Input::SetMouseWrapping(true);
        *hasMoved  = true;
        *unwrapMouse = false;
    }
}

void EditorCamera::HandleKeyMovement(Vector3 *moveStep, bool *hasMoved)
{
    Vector3 m(0);
    if(Input::GetKey(Input::Key::W))
    {
        m += m_keysMoveSpeed * Time::GetDeltaTime() * p_camt->GetForward();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        m -= m_keysMoveSpeed * Time::GetDeltaTime() * p_camt->GetForward();
    }

    if(Input::GetKey(Input::Key::A))
    {
        m -= m_keysMoveSpeed * Time::GetDeltaTime() * p_camt->GetRight();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        m += m_keysMoveSpeed * Time::GetDeltaTime() * p_camt->GetRight();
    }

    *moveStep += m;
    *hasMoved = *hasMoved || (m.Length() != 0);
}

void EditorCamera::HandleLookAtFocus()
{
    if(m_doingLookAt)
    {
        Camera *cam = GetCamera();
        if(p_currentFocus->HasComponent<Transform>())
        {
            Sphere focusBSphere = p_currentFocus->GetBoundingSphere();

            Vector3 thisPos = p_t->GetPosition();
            Vector3 focusPos = focusBSphere.GetCenter();
            Vector3 focusDir = (focusPos - thisPos).Normalized();

            //LookAt Rotation
            if(thisPos != focusPos)
            {
                Quaternion origin = p_t->GetRotation();
                Quaternion dest = Quaternion::LookDirection(focusDir, Vector3::up);
                Quaternion final = Quaternion::Slerp( origin, dest,
                            Time::GetDeltaTime() * m_lookAtRotSpeed);

                p_t->SetLocalRotation(final);
            }

            //LookAt Move
            float minDist = 0.0f;
            float radius = focusBSphere.GetRadius();
            if(cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
            {
                float fov = glm::radians(cam->GetFovDegrees() / 2.0f);
                minDist = radius / std::tan(fov);
            }

            minDist = std::max(minDist, 0.5f); //In case boundingBox is empty
            Vector3 dest = focusPos - (focusDir * minDist);
            p_t->SetPosition( Vector3::Lerp( thisPos, dest,
                            Time::GetDeltaTime() * m_lookAtMoveSpeed));
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

    if(!HandleMouseRotation(&hasMoved, &unwrapMouse)) //Mouse rot with right click
    {
        HandleMousePanning(&hasMoved, &unwrapMouse); //Mouse move with mid click
    }

    HandleWheelZoom(&moveStep, &hasMoved);

    HandleLookAtFocus();
    if(m_doingLookAt)
    {   //Update all needed variables in case we are doing a lookAt.
        UpdateRotationVariables();
    }

    m_keysMoveSpeed += m_keysMoveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    m_keysMoveSpeed = glm::clamp(m_keysMoveSpeed, m_minMoveSpeed, m_maxMoveSpeed);

    if(unwrapMouse)
    {
        Canvas::SetCursor( Qt::ArrowCursor ); //cursor visible
        Input::SetMouseWrapping(false);
    }

    if(!hasMoved )
    {
        m_keysMoveSpeed = 0.0f; //reset speed
    }
    else
    {
        m_doingLookAt = false; //No more lookAt
        p_t->SetPosition(p_t->GetPosition() + moveStep); //Move camera the amount gathered
    }
}

Camera *EditorCamera::GetCamera()
{
    return p_yawNode->GetComponent<Camera>();
}

#ifdef BANG_EDITOR

void EditorCamera::OnTreeHierarchyGameObjectsSelected
    (std::list<GameObject *> &selectedGameObjects)
{
    if(selectedGameObjects.size() != 1) return;

    GameObject *selected = selectedGameObjects.front();
    p_currentFocus = selected;
    m_doingLookAt = false;
}


void EditorCamera::OnTreeHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    NONULL(selected);
    p_currentFocus = selected;
    m_doingLookAt = true;
}
#endif

