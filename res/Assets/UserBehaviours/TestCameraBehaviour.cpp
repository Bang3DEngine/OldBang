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
    Stage *st = GetParent()->GetStage();
    Entity *pyramid = st->GetChild("pyramid");

    time += Time::GetDeltaTime();

    float R = 4.0f;
    Transform *t = GetParent()->GetPart<Transform>();
    /*t->position.x = sin(time) * R;
    t->position.y = 3.0f;// 1.0f * Time::GetDeltaTime();
    t->SetPosition(glm::vec3(t->GetPosition().x, t->GetPosition().y, cos(time) * R));
    */
    t->LookAt(pyramid->GetPart<Transform>()->GetPosition());
    //Logger_Log(t);
}
