#include "TestCameraBehaviour.h"

TestCameraBehaviour::TestCameraBehaviour()
{
}

void TestCameraBehaviour::OnUpdate()
{
    Transform *t = GetParent()->GetPart<Transform>();
    t->position.z += 1.0f * Time::GetDeltaTime();
    t->rotation *= glm::angleAxis(Time::GetDeltaTime(), glm::vec3(1.0f, 1.0f, 0.0f));
    //Logger_Log(t);
}
