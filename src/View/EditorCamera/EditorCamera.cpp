#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
    this->SetName("EditorCamera");

    cam = AddPart<Camera>();
    cam->SetAutoUpdateAspectRatio(true);
    cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    Transform *t = AddPart<Transform>();
    t->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
    t->LookAt(glm::vec3(0));
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::OnUpdate()
{
    Transform *t = GetPart<Transform>();
    if(t == nullptr) return;

    bool doingSomeAction = false;
    bool mustUnlockMouse = true;

    moveSpeed += moveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    moveSpeed = glm::clamp(moveSpeed, minMoveSpeed, maxMoveSpeed);

    glm::vec3 moveStep(0.0f);

    //KEY HANDLING
    if(Input::GetKey(Input::Key::W))
    {
        moveStep += moveSpeed * t->GetForward();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        moveStep -= moveSpeed * t->GetForward();
    }

    if(Input::GetKey(Input::Key::A))
    {
        moveStep -= moveSpeed * t->GetRight();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        moveStep += moveSpeed * t->GetRight();
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


        mouseRotationRads += glm::vec2(mx, my);
        //Orbitting Behaviour
        /*
        t->SetLeftMatrix(glm::rotate(mouseRotationRads.x, t->GetUp()) *
                         glm::rotate(mouseRotationRads.y, t->GetRight()));
        t->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        */
        //

        //Cam rotation Behaviour
        glm::quat rotY = glm::angleAxis(mouseRotationRads.y, glm::vec3(1,0,0));
        glm::quat rotX = glm::angleAxis(mouseRotationRads.x, glm::vec3(0,1,0));
        t->SetRotation( rotY * rotX );
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
                       t->GetRight() * mx +
                       t->GetUp() * my);

        //Canvas::SetCursor(Qt::SizeAllCursor);
        Canvas::SetCursor(Qt::BlankCursor);
        doingSomeAction = true;

        mustUnlockMouse = false;
        Input::LockMouseMovement(true);
    }
    //

    //WHEEL HANDLING
    float mouseWheel = Input::GetMouseWheel();
    if(mouseWheel != 0.0f)
    {
        moveStep += mouseWheelBoost * mouseWheel *
                    moveSpeed * t->GetForward();
        doingSomeAction = true;
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











