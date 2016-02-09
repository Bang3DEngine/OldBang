#include "TestBehaviour.h"
#include "Entity.h"

TestBehaviour::TestBehaviour() : time(.0f)
{
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetOwner()->GetPart<Transform>();
    t->position = glm::vec3(sin(time), -cos(time), sin(time)) * 0.6f;
    t->rotation *= glm::angleAxis(1.0f * Time::GetDeltaTime(), glm::vec3(.0f,1.0f,.0f));
    if(int(time) % 2 == 0)
        t->scale = glm::vec3(0.1f);
    else
        t->scale = glm::vec3(0.3f);

    Logger_Log(t);
    //Logger_Log(t);
}


