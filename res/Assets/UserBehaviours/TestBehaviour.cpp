#include "TestBehaviour.h"

#include "Transform.h"
#include "Logger.h"
#include "Canvas.h"
#include "Stage.h"
#include "Time.h"

TestBehaviour::TestBehaviour() : time(.0f)
{
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetParent()->GetPart<Transform>();
    //t->SetScale(glm::vec3(time));
    //Logger_Log(t);
    t->SetRotation(glm::angleAxis(1.0f * Time::GetDeltaTime(), glm::vec3(1.0f,1.0f,1.0f)) * t->GetRotation());
    //Logger_Log(t);
}


