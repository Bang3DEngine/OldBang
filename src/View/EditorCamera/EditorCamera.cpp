#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
    this->SetName("EditorCamera");

    yawNode = new GameObject("EditorYawNode");
    AddChild(yawNode);

    cam = yawNode->AddComponent<Camera>();
    cam->SetAutoUpdateAspectRatio(true);
    cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    Transform *t = AddComponent<Transform>();
    t->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    camt = yawNode->AddComponent<Transform>();
    //t->LookAt(glm::vec3(0));
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

    glm::vec3 moveStep(0.0f);

    //KEY HANDLING
    if(Input::GetKey(Input::Key::W))
    {
        moveStep += moveSpeed * camt->GetForward();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        moveStep -= moveSpeed * camt->GetForward();
    }

    if(Input::GetKey(Input::Key::A))
    {
        moveStep -= moveSpeed * camt->GetRight();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        moveStep += moveSpeed * camt->GetRight();
    }
    doingSomeAction = glm::length(moveStep) != 0;
    //

    //ROTATION WITH MOUSE HANDLING
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        float mx = -Input::GetMouseAxisX() *
                    mouseRotBoost;
        float my = -Input::GetMouseAxisY() *
                    mouseRotBoost;


        mouseRotationDegrees += glm::vec2(mx, my) * mouseRotBoost;
        //Orbitting Behaviour
        /*
        t->SetLeftMatrix(glm::rotate(mouseRotationRads.x, t->GetUp()) *
                         glm::rotate(mouseRotationRads.y, t->GetRight()));
        t->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        */
        //

        //Cam rotation Behaviour
        //mouseRotX += mx;
        //mouseRotY += my;

        //glm::quat oldRot = t->GetRotation()

        glm::quat rotY = glm::angleAxis(my, t->GetRight());
        t->SetRotation(rotY * t->GetLocalRotation());;

        glm::quat rotX = glm::angleAxis(mx, camt->GetUp());
        camt->SetRotation(rotX * camt->GetLocalRotation());
        Logger_Log(camt->GetEuler());

        //glm::quat rotY = glm::angleAxis(my, glm::vec3(1,0,0));
        //glm::quat rotX = glm::angleAxis(mx, glm::vec3(0,1,0));
        //t->SetRotation( rotY * rotX * t->GetRotation() );

        //glm::vec3 forward = t->GetForward();
        //glm::vec3 localEuler = t->GetEuler();
        //glm::quat undoRoll = glm::angleAxis(glm::radians(localEuler.z), t->GetForward());
        //t->SetRotation(undoRoll * t->GetRotation());

        //glm::vec3 worldEulerNoRoll = glm::inverse(t->GetRotation()) * localEuler;
        //t->SetRotation(worldEulerNoRoll);
        //t->SetRotation(localEuler);
        //Logger_Log(localEuler);// << " -> " << worldEulerNoRoll);

        //glm::vec3 up(0,1,0);
        //Logger_Log("Forward: " << t->GetForward() << ", Up: " << up);
        //float dot = glm::dot(t->GetForward(), glm::vec3(0,1,0));
        /*if(abs(dot) > 0.9f)
        {
            //undo rotation
            //Logger_Log("UNDO");
            //t->SetRotation(oldRot);
        }
        else
        {
        }*/

        //t->SetRotation( glm::vec3(mouseRotationDegrees.y, mouseRotationDegrees.x, 0.0f) );

        //Remove roll from camera
        //glm::vec3 target = t->GetPosition() + t->GetForward() * 99.9f;
        //glm::vec3 dir = t->GetForward();
        //t->LookAt()
        //

        Canvas::SetCursor(Qt::BlankCursor);
        doingSomeAction = true;

        mustUnlockMouse = false;
        Input::LockMouseMovement(true);

    }
    else  //CAM PLANE MOVEMENT  -  MIDDLE PRESS MOVEMENT HANDLING
        if(Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mx = -Input::GetMouseAxisX() * mouseCamPlaneMoveBoost;
        float my = Input::GetMouseAxisY() * mouseCamPlaneMoveBoost;

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
        t->SetPosition(t->GetPosition() + moveStep);
    }
}

Camera *EditorCamera::GetCamera()
{
    return yawNode->GetComponent<Camera>();
}











