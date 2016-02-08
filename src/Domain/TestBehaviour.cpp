#include "TestBehaviour.h"
#include "Entity.h"

TestBehaviour::TestBehaviour()
{
}

void TestBehaviour::OnUpdate()
{
    GetOwner()->GetPart<Transform>()->position.x += 0.01f;
}


