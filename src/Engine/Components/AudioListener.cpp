#include "Bang/AudioListener.h"

#include <AL/al.h>

#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

AudioListener::AudioListener()
{
}

AudioListener::~AudioListener()
{

}

void AudioListener::OnUpdate()
{
    Component::OnUpdate();
    UpdateALProperties();
}

void AudioListener::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);
}

void AudioListener::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);
}

void AudioListener::UpdateALProperties() const
{
    alDistanceModel(AL_LINEAR_DISTANCE);
    //alDistanceModel(AL_EXPONENT_DISTANCE);

    Vector3 at = -gameObject->transform->GetForward();
    Vector3 up = gameObject->transform->GetUp();
    ALfloat listenerOri[] = { at.x, at.y, at.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, listenerOri);
    //alListenerfv(AL_DIRECTION, transform->GetEuler().Data());
    alListenerfv(AL_POSITION, gameObject->transform->GetPosition().Data());
    alListenerfv(AL_VELOCITY, Vector3::Zero.Data());
}
