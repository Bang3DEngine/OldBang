#ifndef RANDOMSCALE_H
#define RANDOMSCALE_H

#include "Behaviour.h"

class RandomScale : public Behaviour
{
private:
    float time = 0.0f;
    float scaleAmplitude = 2.0f;
    Vector3 originalScale;

public:
    void OnStart() override;
    void OnUpdate() override;
};

BANG_BEHAVIOUR_CLASS(RandomScale)

#endif // RANDOMSCALE_H
