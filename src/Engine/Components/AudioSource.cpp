#include "Bang/AudioSource.h"

#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/GameObject.h"
#include "Bang/ICloneable.h"
#include "Bang/AudioManager.h"

USING_NAMESPACE_BANG

AudioSource::AudioSource()
{
}

AudioSource::~AudioSource()
{
    Stop();
}

void AudioSource::OnStart()
{
    Component::OnStart();
    if (IsPlayOnStart())
    {
        Play();
    }
}

void AudioSource::OnUpdate()
{
    Component::OnUpdate();

    if (m_audioClip)
    {
        if (m_currentAudioClipALBufferId != m_audioClip->GetALBufferId())
        {
            SetAudioClip(m_audioClip);
        }
    }

    if (gameObject->transform)
    {
        ALAudioSource::SetPosition(gameObject->transform->GetPosition());
    }
}

void AudioSource::SetAudioClip(AudioClip *audioClip)
{
    m_audioClip = audioClip;
    if (m_audioClip)
    {
        SetALBufferId(audioClip->GetALBufferId());
        m_currentAudioClipALBufferId = audioClip->GetALBufferId();
    }
    else
    {
        SetALBufferId(0);
        m_currentAudioClipALBufferId = 0;
    }
}

void AudioSource::SetPlayOnStart(bool playOnStart)
{
    m_playOnStart = playOnStart;
}

void AudioSource::Play()
{
    Play(0.0f);
}

void AudioSource::Play(float delay)
{
    AudioManager::Play(m_audioClip, this, delay);
}

bool AudioSource::IsPlayOnStart() const { return m_playOnStart; }
AudioClip *AudioSource::GetAudioClip() const { return m_audioClip; }
float AudioSource::GetPlayProgress() const
{
    float secondsOffset;
    alGetSourcef(GetALSourceId(), AL_SEC_OFFSET, &secondsOffset);
    alGetSourcef(GetALSourceId(), AL_SEC_OFFSET, &secondsOffset);
    return secondsOffset / m_audioClip->GetLength();
}

void AudioSource::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    AudioSource *as = SCAST<AudioSource*>(clone);
    as->SetAudioClip( GetAudioClip() );
    as->SetVolume( GetVolume()  );
    as->SetPitch( GetPitch() );
    as->SetRange( GetRange() );
    as->SetLooping( IsLooping() );
    as->SetPlayOnStart( IsPlayOnStart() );
}

void AudioSource::ImportXML(const XMLNode &xml)
{
    Component::ImportXML(xml);

    if (xml.Contains("AudioClip"))
    { SetAudioClip( Resources::Load<AudioClip>( xml.Get<GUID>("AudioClip") ) ); }

    if (xml.Contains("Volume"))
    { SetVolume(xml.Get<float>("Volume")); }

    if (xml.Contains("Pitch"))
    { SetPitch(xml.Get<float>("Pitch")); }

    if (xml.Contains("Range"))
    { SetRange(xml.Get<float>("Range")); }

    if (xml.Contains("Looping"))
    { SetLooping(xml.Get<bool>("Looping")); }

    if (xml.Contains("PlayOnStart"))
    { SetPlayOnStart(xml.Get<bool>("PlayOnStart")); }
}

void AudioSource::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    AudioClip *audioClip = GetAudioClip();
    GUID audioClipGUID = audioClip ? audioClip->GetGUID() : GUID::Empty();
    xmlInfo->Set("AudioClip",   audioClipGUID);
    xmlInfo->Set("Volume",      GetVolume());
    xmlInfo->Set("Pitch",       GetPitch());
    xmlInfo->Set("Range",       GetRange());
    xmlInfo->Set("Looping",     IsLooping());
    xmlInfo->Set("PlayOnStart", IsPlayOnStart());
}
