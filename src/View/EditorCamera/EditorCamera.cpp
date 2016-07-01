#include "EditorCamera.h"

#include "Canvas.h"

EditorCamera::EditorCamera() : EditorGameObject("EditorCamera")
{
    yawNode = new EditorGameObject("EditorYawNode");
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

    mouseRotDegreesPerPixel.x = 180.0f / cw;
    mouseRotDegreesPerPixel.y = 180.0f / ch;

    mousePanPerPixel = glm::vec2(0.1f, 0.1f);
    if(currentFocus )
    {
        Transform *ft = currentFocus->GetComponent<Transform>();
        if(ft )
        {
            Vector3 focusPoint = ft->GetPosition();
            float d = Vector3::Distance(focusPoint, t->GetPosition());
            float ar = c->GetAspectRatio();
            float halfFov = glm::radians(cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = glm::tan(halfFov) * d;
            mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / ch;
            mousePanPerPixel.x = mousePanPerPixel.y * ch * ar / cw;
        }
    }
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
            *moveStep += mouseZoomPerDeltaWheel * mouseWheel * camt->GetForward();
            *hasMoved  = true;
        }
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        glm::vec2 delta = -Input::GetMouseDelta() * mouseRotDegreesPerPixel;
        mouseRotDegreesAccum += delta;

        t->SetLocalRotation(startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(glm::radians(mouseRotDegreesAccum.x),
                                                Vector3::up);
        t->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(glm::radians(mouseRotDegreesAccum.y),
                                                camt->GetRight());
        t->RotateLocal(rotY);

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
        glm::vec2 delta = -Input::GetMouseDelta() * mousePanPerPixel;
        delta.y *= -1.0f;

        t->Translate(camt->GetRight() * delta.x + camt->GetUp() * delta.y);

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
        m += keysMoveSpeed * Time::GetDeltaTime() * camt->GetForward();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        m -= keysMoveSpeed * Time::GetDeltaTime() * camt->GetForward();
    }

    if(Input::GetKey(Input::Key::A))
    {
        m -= keysMoveSpeed * Time::GetDeltaTime() * camt->GetRight();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        m += keysMoveSpeed * Time::GetDeltaTime() * camt->GetRight();
    }

    *moveStep += m;
    *hasMoved = *hasMoved || (m.Length() != 0);
}

void EditorCamera::HandleLookAtFocus()
{
    if(doingLookAt)
    {
        Camera *cam = GetCamera();
        if(currentFocus->HasComponent<Transform>())
        {
            Sphere focusBSphere = currentFocus->GetBoundingSphere();

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

                t->SetLocalRotation(final);
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
    if(doingLookAt)
    {   //Update all needed variables in case we are doing a lookAt.
        UpdateRotationVariables();
    }

    keysMoveSpeed += keysMoveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    keysMoveSpeed = glm::clamp(keysMoveSpeed, minMoveSpeed, maxMoveSpeed);

    if(unwrapMouse)
    {
        Canvas::SetCursor( Qt::ArrowCursor ); //cursor visible
        Input::SetMouseWrapping(false);
    }

    if(!hasMoved )
    {
        keysMoveSpeed = 0.0f; //reset speed
    }
    else
    {
        doingLookAt = false; //No more lookAt
        t->SetPosition(t->GetPosition() + moveStep); //Move camera the amount gathered
    }
}

Camera *EditorCamera::GetCamera()
{
    return yawNode->GetComponent<Camera>();
}

#ifdef BANG_EDITOR

void EditorCamera::OnTreeHierarchyGameObjectsSelected
    (std::list<GameObject *> &selectedGameObjects)
{
    if(selectedGameObjects.size() != 1) return;

    GameObject *selected = selectedGameObjects.front();
    currentFocus = selected;
    doingLookAt = false;
}


void EditorCamera::OnTreeHierarchyGameObjectDoubleClicked(GameObject *selected)
{
    NONULL(selected);
    currentFocus = selected;
    doingLookAt = true;
}
#endif

