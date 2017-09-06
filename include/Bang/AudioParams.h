#ifndef AUDIOPARAMS_H
#define AUDIOPARAMS_H

#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

struct AudioParams
{
    Vector3 position;
    float volume, delay, pitch, range;
    bool looping;

    AudioParams(const Vector3& _position = Vector3::Zero,
                float _volume = 1.0f, float _delay = 0.0f, float _pitch = 1.0f,
                float _range = Math::Infinity<float>(), bool _looping = false)
        : position(_position),
          volume(_volume), delay(_delay), pitch(_pitch),
          range(_range), looping(_looping)
    {
    }
};

NAMESPACE_BANG_END

#endif // AUDIOPARAMS_H
