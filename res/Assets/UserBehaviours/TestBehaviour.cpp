#include "TestBehaviour.h"

#include "Transform.h"
#include "Logger.h"
#include "Canvas.h"
#include "Stage.h"
#include "Time.h"

TestBehaviour::TestBehaviour()
{
    time = 0.0f;
    randomAxis = glm::vec3( float(rand()%2000-1000)/1000,
                            float(rand()%2000-1000)/1000,
                            float(rand()%2000-1000)/1000);
    randomAxis = glm::normalize(randomAxis);
}

void TestBehaviour::OnStart()
{
    GetOwner()->GetPart<MeshRenderer>()->GetMaterial()->SetDiffuseColor(glm::vec4( float(rand()%2000-1000)/1000,
                                                                         float(rand()%2000-1000)/1000,
                                                                         float(rand()%2000-1000)/1000,
                                                                         0.5f));
}

void TestBehaviour::OnUpdate()
{
    time += Time::GetDeltaTime();
    Transform *t = GetOwner()->GetPart<Transform>();
    t->SetRotation(glm::angleAxis(1.0f * Time::GetDeltaTime(), randomAxis) * t->GetRotation());
}

void TestBehaviour::OnDestroy()
{

}


