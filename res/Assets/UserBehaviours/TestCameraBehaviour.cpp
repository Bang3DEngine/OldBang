#include "TestCameraBehaviour.h"

#include "WindowMain.h"

#include "GameObject.h"
#include "Transform.h"
#include "Logger.h"
#include "Canvas.h"
#include "Scene.h"
#include "Box.h"
#include "Time.h"

TestCameraBehaviour::TestCameraBehaviour()
{
    //this->SetFilepath(__FILE__);
    //time = 0.0f;
    Logger_Log("HOLA");
}


void TestCameraBehaviour::OnUpdate()
{
    Logger_Log(GetOwner());
    GetOwner()->GetComponent<Transform>()->Translate(GetOwner()->GetComponent<Transform>()->GetForward());
    /*
    Scene *st = GetParent()->GetScene();
    GameObject *pyramid = st->GetChild("pyramid");


    if(pyramid != nullptr)
    {
        Transform *t = GetParent()->GetComponent<Transform>();
        float R = 4.0f;
        time += Time::GetDeltaTime();
        t->SetPosition(Vector3(t->GetPosition().x,
                                 pyramid->GetComponent<Transform>()->GetPosition().y + sin(time) * R,
                                 pyramid->GetComponent<Transform>()->GetPosition().z + cos(time) * R));

        //Logger_Log(t);
        t->LookAt(pyramid->GetComponent<Transform>()->GetPosition());
    }
        */
}

extern "C" Behaviour *CreateDynamically(WindowMain *singletonWindowMain)
{
    Logger_Log("Creating behaviour dinamycally...");

    WindowMain::Init(singletonWindowMain);
    Behaviour *b = new TestCameraBehaviour();

    Logger_Log("Dynamic behaviour successfully created!");
    return b;
}

extern "C" void DeleteDynamically(int *b)
{
    delete b;
}

