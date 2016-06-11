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
}


void TestCameraBehaviour::OnUpdate()
{
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
    Logger_Log("Creating D...");

    WindowMain::Init(singletonWindowMain);
    Behaviour *b = new Behaviour();

    Logger_Log("Finished!");
    return b;
}

extern "C" void DeleteDynamically(int *b)
{
    delete b;
}

