#include "EditorCamera.h"

EditorCamera::EditorCamera() : EditorGameObject("EditorCamera")
{
    yawNode = new GameObject("EditorYawNode");
    AddChild(yawNode);

    cam = yawNode->AddComponent<Camera>();
    cam->SetAutoUpdateAspectRatio(true);
    cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    Transform *t = AddComponent<Transform>();
    t->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

    camt = yawNode->AddComponent<Transform>();
    //t->LookAt(Vector3(0));
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::OnUpdate()
{
    Transform *t = GetComponent<Transform>();

    bool doingSomeAction = false;
    bool mustUnlockMouse = true;

    moveSpeed += moveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    moveSpeed = glm::clamp(moveSpeed, minMoveSpeed, maxMoveSpeed);

    Vector3 moveStep(0.0f);

    //KEY HANDLING
    if(Input::GetKey(Input::Key::W))
    {
        moveStep += moveSpeed  * Time::GetDeltaTime() * camt->GetForward();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        moveStep -= moveSpeed * Time::GetDeltaTime() * camt->GetForward();
    }

    if(Input::GetKey(Input::Key::A))
    {
        moveStep -= moveSpeed * Time::GetDeltaTime() * camt->GetRight();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        moveStep += moveSpeed * Time::GetDeltaTime() * camt->GetRight();
    }
    doingSomeAction = moveStep.Length() != 0;
    //

    //ROTATION WITH MOUSE HANDLING
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        float mx = -Input::GetMouseAxisX() *
                    mouseRotBoost  * Time::GetDeltaTime();
        float my = -Input::GetMouseAxisY() *
                    mouseRotBoost  * Time::GetDeltaTime();


        mouseRotationDegrees += glm::vec2(mx, my) * mouseRotBoost;
        //Orbitting Behaviour
        /*
        t->SetLeftMatrix(glm::rotate(mouseRotationRads.x, t->GetUp()) *
                         glm::rotate(mouseRotationRads.y, t->GetRight()));
        t->LookAt(Vector3(0.0f, 0.0f, 0.0f));
        */
        //

        Quaternion rotX = Quaternion::AngleAxis(mouseRotationDegrees.x,
                                                Vector3(0,1,0));
        t->SetRotation( rotX  );
        Quaternion rotY = Quaternion::AngleAxis(mouseRotationDegrees.y,
                                                camt->GetRight());
        t->Rotate( rotY );

        Canvas::SetCursor(Qt::BlankCursor);
        doingSomeAction = true;

        mustUnlockMouse = false;
        Input::LockMouseMovement(true);

    }
    else  //CAM PLANE MOVEMENT  -  MIDDLE PRESS MOVEMENT HANDLING
        if(Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mx = -Input::GetMouseAxisX() * mouseCamPlaneMoveBoost * Time::GetDeltaTime();
        float my = Input::GetMouseAxisY() * mouseCamPlaneMoveBoost * Time::GetDeltaTime();

        t->SetPosition(t->GetPosition()   +
                       camt->GetRight() * mx +
                       camt->GetUp() * my);

        //Canvas::SetCursor(Qt::SizeAllCursor);
        Canvas::SetCursor(Qt::BlankCursor);
        doingSomeAction = true;

        mustUnlockMouse = false;
        Input::LockMouseMovement(true);
    }
    //

    //WHEEL HANDLING
    if(!Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mouseWheel = Input::GetMouseWheel();
        if(mouseWheel != 0.0f)
        {
            moveStep += mouseWheelBoost * mouseWheel *
                        moveSpeed * camt->GetForward();
            doingSomeAction = true;
        }
    }
    //

    if(Input::GetKey(Input::Key::X))
    {
        t->SetRotation( Quaternion::Slerp(t->GetRotation(),
                                         Quaternion::LookDirection(Vector3::right),
                                         Time::GetDeltaTime()
                                        )
                      );
    }
    if(Input::GetKey(Input::Key::Z))
    {
        t->SetRotation( Quaternion::Slerp(t->GetRotation(),
                                         Quaternion::LookDirection(Vector3::up),
                                         Time::GetDeltaTime()
                                        )
                      );
    }


    if(mustUnlockMouse)
    {
        Input::LockMouseMovement(false);
    }

    if(!doingSomeAction)
    {
        moveSpeed = 0.0f; //reset speed
        Canvas::SetCursor( Qt::ArrowCursor ); //reset cursor
    }
    else
    {
        currentLookAtFocus = nullptr; //No more lookAt
        t->SetPosition(t->GetPosition() + moveStep);
    }

    if(Input::GetKeyDown(Input::Key::T))
    {
        GetScene()->DebugDrawLine(Vector3(0.0f), Vector3(5.0f));
    }

    //Look At Focus
    if(currentLookAtFocus != nullptr)
    {
        Camera *cam = yawNode->GetComponent<Camera>();
        Transform *ft = currentLookAtFocus->GetComponent<Transform>();
        if(ft != nullptr)
        {
            Box focusBBox = currentLookAtFocus->GetBoundingBox();
            //Matrix4 viewMat; cam->GetViewMatrix(viewMat);
            //focusBBox = viewMat * focusBBox;

            Vector3 thisPos = t->GetPosition();
            Vector3 focusPos = focusBBox.GetCenter(); //ft->GetPosition();
            Vector3 focusDir = (focusPos - thisPos).Normalized();

            //LookAt Rotation
            if(thisPos != focusPos)
            {
                Quaternion origin = t->GetRotation();
                Quaternion dest = Quaternion::LookDirection(focusDir);
                Quaternion final = Quaternion::Slerp( origin, dest,
                            Time::GetDeltaTime() * lookAtRotSpeed);

                t->SetRotation(final);
            }

            //Move
            float halfHeight = focusBBox.GetHeight() / 2;
            float d = Vector3::Distance(thisPos, focusPos);
            float minDist;
            if(cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
            {
                float fov = glm::radians(cam->GetFovDegrees());
                minDist = halfHeight / std::tan(fov);
            }

            minDist = std::max(minDist, 0.5f); //In case boundingBox is empty
            t->SetPosition(
                        Vector3::Lerp(
                            thisPos,
                            focusPos - focusDir * minDist,
                            Time::GetDeltaTime() * lookAtMoveSpeed)
                        );
        }
    }


    //
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










