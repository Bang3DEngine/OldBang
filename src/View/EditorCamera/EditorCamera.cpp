#include "EditorCamera.h"

EditorCamera::EditorCamera()
{
    this->SetName("EditorCamera");

    cam = new Camera();
    AddPart(cam);

    cam->SetAutoUpdateAspectRatio(true);
    cam->SetProjectionMode(Camera::ProjectionMode::Perspective);

    Transform *t = new Transform();
    AddPart(t);
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::OnUpdate()
{
    Transform *t = GetPart<Transform>();
    if(t == nullptr) return;

    bool doingSomeAction = false;

    moveSpeed += moveAccel; //TODO: must do this in FixedUpdate which does not exist yet
    moveSpeed = glm::clamp(moveSpeed, minMoveSpeed, maxMoveSpeed);

    glm::vec3 moveStep(0.0f);

    //KEY HANDLING
    if(Input::GetKey(Input::Key::W))
    {
        moveStep += moveSpeed * t->GetForward() * Time::GetDeltaTime();
    }
    else if(Input::GetKey(Input::Key::S))
    {
        moveStep -= moveSpeed * t->GetForward() * Time::GetDeltaTime();
    }

    if(Input::GetKey(Input::Key::A))
    {
        moveStep -= moveSpeed * t->GetRight() * Time::GetDeltaTime();
    }
    else if(Input::GetKey(Input::Key::D))
    {
        moveStep += moveSpeed * t->GetRight() * Time::GetDeltaTime();
    }
    doingSomeAction = glm::length(moveStep) != 0;
    //

    //ROTATION WITH MOUSE HANDLING
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        float mx = -Input::GetMouseAxisX() * mouseRotBoost * Time::GetDeltaTime();
        float my = -Input::GetMouseAxisY() * mouseRotBoost * Time::GetDeltaTime();

        /* Orbitting Behaviour
        mouseRotationRads += glm::vec2(mx, my);
        t->SetLeftMatrix(glm::rotate(mouseRotationRads.x, t->GetUp()) *
                         glm::rotate(mouseRotationRads.y, t->GetRight()));
        t->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        */

        //Cam rotation Behaviour
        glm::quat rotX = glm::angleAxis(mx, t->GetUp());
        glm::quat rotY = glm::angleAxis(my, t->GetRight());
        t->SetRotation( rotX * rotY * t->GetRotation() );
        Canvas::SetCursor(Qt::BlankCursor);
        doingSomeAction = true;
    }
    //

    //CAM PLANE MOVEMENT  -  MIDDLE PRESS MOVEMENT HANDLING
    if(Input::GetMouseButton(Input::MouseButton::MMiddle))
    {
        float mx = -Input::GetMouseAxisX() * mouseCamPlaneMoveBoost *
                    Time::GetDeltaTime();
        float my =  Input::GetMouseAxisY() * mouseCamPlaneMoveBoost *
                    Time::GetDeltaTime();

        t->SetPosition(t->GetPosition()   +
                       t->GetRight() * mx +
                       t->GetUp() * my);

        Canvas::SetCursor(Qt::SizeAllCursor);
        doingSomeAction = true;
    }
    //

    //WHEEL HANDLING
    float mouseWheel = Input::GetMouseWheel();
    if(mouseWheel != 0.0f)
    {
        moveStep += mouseWheelBoost * mouseWheel *
                    moveSpeed * t->GetForward() * Time::GetDeltaTime();
        doingSomeAction = true;
    }
    //

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











