#include "EditorCamera.h"

#include "Canvas.h"

EditorCamera::EditorCamera() : EditorGameObject("EditorCamera")
{
    yawNode = new GameObject("EditorYawNode");
    AddChild(yawNode);

    cam = yawNode->AddComponent<Camera>();
    cam->SetAutoUpdateAspectRatio(true);
    cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    t = AddComponent<Transform>();
    t->SetPosition(Vector3(10.0f, 10.0f, 10.0f));
    t->LookAt(Vector3(0));
    UpdateRotationVariables();

    camt = yawNode->AddComponent<Transform>();
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::AdjustSpeeds()
{
    Canvas *c = Canvas::GetInstance();
    int cw = c->GetWidth();
    int ch = c->GetHeight();
    mouseRotDegreesPerPixel.x = 360.0f / cw;
    mouseRotDegreesPerPixel.y = 360.0f / ch;
}

void EditorCamera::UpdateRotationVariables()
{
    mouseRotDegreesAccum = glm::vec2(0.0f);
    startingRotation = t->GetLocalRotation();
}

void EditorCamera::HandleWheelZoom(Vector3 *moveStep, bool *hasMoved)
{
    if(!Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mouseWheel = Input::GetMouseWheel();
        if(mouseWheel != 0.0f)
        {
            *moveStep += mouseZoomPerDeltaWheel * mouseWheel *
                         moveSpeed * camt->GetForward();
            *hasMoved  = true;
        }
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        Input::SetMouseWrapping(true);

        float mx = -Input::GetMouseDeltaX() * mouseRotDegreesPerPixel.x;
        float my = -Input::GetMouseDeltaY() * mouseRotDegreesPerPixel.y;
        glm::vec2 delta = glm::vec2(mx, my);
        mouseRotDegreesAccum += delta;

        t->SetRotation(startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(glm::radians(mouseRotDegreesAccum.x),
                                                Vector3::up);
        t->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(glm::radians(mouseRotDegreesAccum.y),
                                                camt->GetRight());
        t->Rotate(rotY);

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
        Input::SetMouseWrapping(true);

        float mx = -Input::GetMouseAxisX() * mousePanPerPixel * Time::GetDeltaTime();
        float my = Input::GetMouseAxisY() * mousePanPerPixel * Time::GetDeltaTime();

        t->SetPosition(t->GetPosition()   +
                       camt->GetRight() * mx +
                       camt->GetUp() * my);

        Canvas::SetCursor(Qt::SizeAllCursor);
        *hasMoved  = true;

        *unwrapMouse = false;
    }
}

void EditorCamera::HandleKeyMovement(Vector3 *moveStep, bool *hasMoved)
{
    Vector3 m(0);
    if(Input::GetKey(Input::Key::W))
    {
        m += moveSpeed  * Time::GetDeltaTime() * camt->GetForward();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        m -= moveSpeed * Time::GetDeltaTime() * camt->GetForward();
    }

    if(Input::GetKey(Input::Key::A))
    {
        m -= moveSpeed * Time::GetDeltaTime() * camt->GetRight();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        m += moveSpeed * Time::GetDeltaTime() * camt->GetRight();
    }

    *moveStep += m;
    *hasMoved = *hasMoved || (m.Length() != 0);
}

void EditorCamera::HandleLookAtFocus()
{
    if(currentLookAtFocus != nullptr)
    {
        Camera *cam = GetCamera();
        if(currentLookAtFocus->HasComponent<Transform>())
        {
            Sphere focusBSphere = currentLookAtFocus->GetBoundingSphere();

            Vector3 thisPos = t->GetPosition();
            Vector3 focusPos = focusBSphere.GetCenter();
            Vector3 focusDir = (focusPos - thisPos).Normalized();

            //LookAt Rotation
            if(thisPos != focusPos)
            {
                Quaternion origin = t->GetRotation();
                Quaternion dest = Quaternion::LookDirection(focusDir, Vector3::up);
                Quaternion final = Quaternion::Slerp( origin, dest,
                            Time::GetDeltaTime() * lookAtRotSpeed);

                t->SetRotation(final);
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
            t->SetPosition( Vector3::Lerp( thisPos, dest,
                            Time::GetDeltaTime() * lookAtMoveSpeed));
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
    if(currentLookAtFocus != nullptr)
    {   //Update all needed variables in case we are doing a lookAt.
        UpdateRotationVariables();
    }

    moveSpeed += moveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    moveSpeed = glm::clamp(moveSpeed, minMoveSpeed, maxMoveSpeed);

    if(unwrapMouse)
    {
        Canvas::SetCursor( Qt::ArrowCursor ); //cursor visible
        Input::SetMouseWrapping(false);
    }

    if(!hasMoved )
    {
        moveSpeed = 0.0f; //reset speed
    }
    else
    {
        currentLookAtFocus = nullptr; //No more lookAt
        t->SetPosition(t->GetPosition() + moveStep); //Move camera the amount gathered
    }
}

Camera *EditorCamera::GetCamera()
{
    return yawNode->GetComponent<Camera>();
}

#ifdef BANG_EDITOR
void EditorCamera::OnTreeHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    if(selected == nullptr) return;

    currentLookAtFocus = selected;
}
#endif

