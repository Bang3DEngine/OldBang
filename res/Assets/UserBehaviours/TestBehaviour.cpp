#include "TestBehaviour.h"

#include "Transform.h"
#include "Logger.h"
#include "Canvas.h"
#include "Stage.h"
#include "Time.h"

TestBehaviour::TestBehaviour()
{
    time = 0.0f;
    /*
    randomAxis = glm::vec3( float(rand()%1000)/1000, float(rand()%1000)/1000, float(rand()%1000)/1000);
    randomAxis = glm::normalize(randomAxis);
    */
}

void TestBehaviour::OnStart()
{
}

void TestBehaviour::OnUpdate()
{
    /*
    time += Time::GetDeltaTime();
    Transform *t = GetParent()->GetPart<Transform>();
    t->SetRotation(glm::angleAxis(1.0f * Time::GetDeltaTime(), randomAxis) * t->GetRotation());
    */
}

void TestBehaviour::OnDestroy()
{

}


