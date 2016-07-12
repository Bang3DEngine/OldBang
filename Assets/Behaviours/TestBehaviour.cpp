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

    int asljd = 0;
}

void TestBehaviour::OnStart()
{
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = gameObject->GetComponent<Transform>(); NONULL(t);

    float rotSpeed = 50.0f;
    if (Input::GetKey(Input::Key::A))
    {
        t->RotateLocalEuler(Vector3::up * rotSpeed * Time::GetDeltaTime());
    }
    else if (Input::GetKey(Input::Key::D))
    {
        t->RotateLocalEuler(Vector3::up * -rotSpeed * Time::GetDeltaTime());
    }

    float speed = 50.0f;
    if (Input::GetKey(Input::Key::W))
    {
        t->Translate(t->GetForward() * speed * Time::GetDeltaTime());
    }
    else if (Input::GetKey(Input::Key::S))
    {
        t->Translate(t->GetForward() * -speed * Time::GetDeltaTime());
    }

    t->RotateLocalEuler(Vector3::up * rotSpeed * Time::GetDeltaTime() * 0.3f);
}

void TestBehaviour::OnDestroy()
{

}


