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
        float r = glm::distance(t->GetPosition(), glm::vec3(0.0f));
        float mx = Input::GetMouseAxisX() * mouseRotBoost * Time::GetDeltaTime();
        float my = Input::GetMouseAxisY() * mouseRotBoost * Time::GetDeltaTime();

        float dx = r * sin(mx) * cos(my);
        float dy = r * cos(my) * sin(mx);
        float dz = r * cos(mx);

        glm::vec3 newPosition = glm::vec3(dx, dy, dz);
        t->SetPosition(newPosition);
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











