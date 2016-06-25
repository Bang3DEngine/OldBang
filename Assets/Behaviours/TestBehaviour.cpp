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
    Logger_Log("ON START!");
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetOwner()->GetComponent<Transform>(); NONULL(t);

    float speed = 5.0f;
    Logger_Log("AAAAAAAAA");
    if (Input::GetKey(Input::Key::W))
    {
        Logger_Log("bbbbbbbbbbbb");
        t->Rotate(Vector3::right * speed);
    }
    else if (Input::GetKey(Input::Key::S))
    {
        Logger_Log("cccccccccccc");
        t->Rotate(Vector3::left * speed);
    }
}

void TestBehaviour::OnDestroy()
{

}


