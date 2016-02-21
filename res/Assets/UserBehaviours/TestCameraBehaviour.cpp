#include "TestCameraBehaviour.h"

#include "Transform.h"
#include "Logger.h"
#include "Canvas.h"
#include "Stage.h"
#include "Time.h"

TestCameraBehaviour::TestCameraBehaviour()
{
    time = 0.0f;
}


void TestCameraBehaviour::OnUpdate()
{
    /*
    Stage *st = GetParent()->GetStage();
    Entity *pyramid = st->GetChild("pyramid");


    if(pyramid != nullptr)
    {
        Transform *t = GetParent()->GetPart<Transform>();
        float R = 4.0f;
        time += Time::GetDeltaTime();
        t->SetPosition(glm::vec3(t->GetPosition().x,
                                 pyramid->GetPart<Transform>()->GetPosition().y + sin(time) * R,
                                 pyramid->GetPart<Transform>()->GetPosition().z + cos(time) * R));

        //Logger_Log(t);
        t->LookAt(pyramid->GetPart<Transform>()->GetPosition());
    }
        */
}
