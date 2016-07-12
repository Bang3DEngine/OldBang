#include "TestBehaviour.h"

#include "ShaderContract.h"
#include "Transform.h"
#include "Material.h"
#include "Logger.h"
#include "Canvas.h"
#include "Scene.h"
#include "Time.h"

TestBehaviour::TestBehaviour()
{
    time = 0.0f;

    int r = rand()%3;
    if(r == 0) randomAxis = Vector3(1.0, 0.0, 0.0);
    if(r == 1) randomAxis = Vector3(0.0, 1.0, 0.0);
    if(r == 2) randomAxis = Vector3(0.0, 0.0, 1.0);

    randomAxis = randomAxis.Normalized();
}

void TestBehaviour::OnStart()
{
    Behaviour::OnStart();
}

void TestBehaviour::OnUpdate()
{
    Behaviour::OnUpdate();

    time += Time::deltaTime;

    float rotSpeed = 50.0f;
    if (Input::GetKey(Input::Key::A))
    {
        transform->RotateLocalEuler(Vector3::up * rotSpeed * Time::deltaTime);
    }
    else if (Input::GetKey(Input::Key::D))
    {
        transform->RotateLocalEuler(Vector3::up * -rotSpeed * Time::deltaTime);
    }

    float speed = 50.0f;
    if (Input::GetKey(Input::Key::W))
    {
        transform->Translate(transform->GetForward() * speed * Time::deltaTime);
    }
    else if (Input::GetKey(Input::Key::S))
    {
        transform->Translate(transform->GetForward() * -speed * Time::deltaTime);
    }

    transform->RotateLocalEuler(Vector3::up * rotSpeed * Time::deltaTime * 0.3f);
}

void TestBehaviour::OnDestroy()
{

}


