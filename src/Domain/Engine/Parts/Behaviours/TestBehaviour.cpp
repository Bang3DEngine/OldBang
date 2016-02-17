#include "TestBehaviour.h"

TestBehaviour::TestBehaviour() : time(.0f)
{
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetParent()->GetPart<Transform>();
    t->rotation = glm::angleAxis(1.0f * Time::GetDeltaTime(), glm::vec3(0.0f,1.0f,.0f)) * t->rotation;
//    Logger_Log(t);
}


