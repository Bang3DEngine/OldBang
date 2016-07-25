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
    gameObject->transform->SetLocalScale(originalScale + (sin(time) * 0.5f + 0.51f) * scaleAmplitude);
}

