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
    //

    //ROTATION WITH MOUSE HANDLING
    if(Input::GetMouseButton(Input::MouseButton::MRight))
    {
        //We move the camera in x, y in view space.
        //Then we translate from view to world space.

        float mx = Input::GetMouseAxisX() * mouseRotBoost * Time::GetDeltaTime();
        float my = Input::GetMouseAxisY() * mouseRotBoost * Time::GetDeltaTime();

        glm::mat4 viewMatrix, viewMatrixInverse;
        cam->GetViewMatrix(viewMatrix);
        viewMatrixInverse = glm::inverse(viewMatrix);

        glm::vec3 posViewSpace = ( viewMatrix *
                                   glm::vec4(t->GetPosition(), 1.0f) ).xyz();
        glm::vec3 newPosViewSpace = posViewSpace + glm::vec3(-mx, -my, 0.0f);
        glm::vec3 newPosWorldSpace = ( viewMatrixInverse *
                                       glm::vec4(newPosViewSpace, 1.0f) ).xyz();

        t->SetPosition(newPosWorldSpace);
        t->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    //

    //WHEEL HANDLING
    float mouseWheel = Input::GetMouseWheel();
    if(mouseWheel != 0.0f)
    {
        moveStep += mouseWheelBoost * mouseWheel *
                    moveSpeed * t->GetForward() * Time::GetDeltaTime();
    }
    //

    bool someKeyPressed = glm::length(moveStep) != 0.0f;
    if(!someKeyPressed)
    {
        moveSpeed = 0.0f; //reset speed
    }
    else
    {
        t->SetPosition(t->GetPosition() + moveStep);
    }
}











