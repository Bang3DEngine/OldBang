#include "AudioListener.h"

#include <AL/al.h>

#include "XMLNode.h"
#include "Transform.h"

AudioListener::AudioListener()
{
}

AudioListener::~AudioListener()
{

}

String AudioListener::GetName() const
{
    return "AudioListener";
}

void AudioListener::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    AudioListener *al = Object::SCast<AudioListener>(clone);
}

ICloneable *AudioListener::Clone() const
{
    AudioListener *al = new AudioListener();
    CloneInto(al);
    return al;
}

void AudioListener::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
}

void AudioListener::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName( GetName() );
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
    Vector3 up = up = transform->GetUp();
    ALfloat listenerOri[] = { at.x, at.y, at.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, listenerOri);
    //alListenerfv(AL_DIRECTION, transform->GetEuler().Values());
    alListenerfv(AL_POSITION,    transform->GetPosition().Values());
    alListenerfv(AL_VELOCITY,    Vector3::Zero.Values());
}
