#ifndef AUDIOPLAYPROPERTIES
#define AUDIOPLAYPROPERTIES

#include "Vector3.h"

struct AudioPlayProperties
{
    float delayInSeconds   = 0.0f;
    float volume           = 1.0f;
    float pitch            = 1.0f;
    bool looping           = false;
    Vector3 sourceVelocity = Vector3::Zero;
    Vector3 sourcePosition = Vector3::Zero;
};

#endif // AUDIOPLAYPROPERTIES

