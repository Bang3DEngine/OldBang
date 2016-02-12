#include "TestCameraBehaviour.h"

TestCameraBehaviour::TestCameraBehaviour()
{
}

void TestCameraBehaviour::OnUpdate()
{
    Stage *st = GetParent()->GetStage();
    Entity *pyramid = st->GetChild("pyramid");

    Transform *t = GetParent()->GetPart<Transform>();
    t->LookAt(pyramid->GetPart<Transform>()->position);
    //Logger_Log(t);
}
