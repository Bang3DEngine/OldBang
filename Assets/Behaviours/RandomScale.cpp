#include "RandomScale.h"

void RandomScale::OnStart()
{
    Behaviour::OnStart();

    originalScale = gameObject->transform->GetLocalScale();
}


void RandomScale::OnUpdate()
{
    Behaviour::OnUpdate();

    time += Time::GetDeltaTime();
    transform->SetLocalEuler(0.0f, time * 30.0f, 0.0f);
    //gameObject->transform->SetLocalScale(originalScale + (sin(time) * 0.5f + 0.51f) * scaleAmplitude);
}

