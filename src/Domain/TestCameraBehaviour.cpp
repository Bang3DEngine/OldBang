#include "TestCameraBehaviour.h"

TestCameraBehaviour::TestCameraBehaviour()
{
}

void TestCameraBehaviour::OnUpdate()
{
    Stage *st = GetParent()->GetStage();
    Entity *pyramid = st->GetChild("pyramid");

    Transform *t = GetParent()->GetPart<Transform>();
    t->position = glm::vec3(3.0f, 5.0f, 5.0f);
    t->LookAt(pyramid->GetPart<Transform>()->position);
    //Logger_Log(t);
}
