#include "Bang/AudioListener.h"

#include <AL/al.h>

#include "Bang/XMLNode.h"
#include "Bang/Transform.h"

AudioListener::AudioListener()
{
}

AudioListener::~AudioListener()
{

}

void AudioListener::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);
}

void AudioListener::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);
}

void AudioListener::OnUpdate()
{
    Component::OnUpdate();
    UpdateALProperties();
}

void AudioListener::UpdateALProperties() const
{
    alDistanceModel(AL_LINEAR_DISTANCE);
    //alDistanceModel(AL_EXPONENT_DISTANCE);

    Vector3 at = -transform->GetForward();
    Vector3 up = transform->GetUp();
    ALfloat listenerOri[] = { at.x, at.y, at.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, listenerOri);
    //alListenerfv(AL_DIRECTION, transform->GetEuler().Values());
    alListenerfv(AL_POSITION,    transform->GetPosition().Values());
    alListenerfv(AL_VELOCITY,    Vector3::Zero.Values());
}