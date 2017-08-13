#include "Bang/AudioSource.h"

#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/ICloneable.h"
#include "Bang/AudioManager.h"
#include "Bang/Resources.h"

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

    if (m_currentAudioClipALBufferId != m_audioClip->GetALBufferId())
    {
        SetAudioClip(m_audioClip);
    }

    if (transform)
    {
        ALAudioSource::SetPosition(transform->GetPosition());
    }
}

void AudioSource::SetAudioClip(AudioClip *audioClip)
{
    m_audioClip = audioClip;
    SetALBufferId(audioClip->GetALBufferId());
    m_currentAudioClipALBufferId = audioClip->GetALBufferId();
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

void AudioSource::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);

    AudioClip *audioClip = GetAudioClip();
    Path audioClipFilepath = audioClip ? audioClip->GetFilepath() : Path();
    Path newAudioClipFilepath = xmlInfo.Get<Path>("AudioClip");
    if (audioClipFilepath != newAudioClipFilepath)
    {
        SetAudioClip( Resources::Load<AudioClip>(newAudioClipFilepath) );
    }
    SetVolume(xmlInfo.Get<float>("Volume"));
    SetPitch(xmlInfo.Get<float>("Pitch"));
    SetRange(xmlInfo.Get<float>("Range"));
    SetLooping(xmlInfo.Get<bool>("Looping"));
    SetPlayOnStart(xmlInfo.Get<bool>("PlayOnStart"));
}

void AudioSource::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);

    AudioClip *audioClip = GetAudioClip();
    Path audioClipFilepath = audioClip ? audioClip->GetFilepath() : Path::Empty;
    xmlInfo->Set("AudioClip", audioClipFilepath);
    xmlInfo->Set("Volume",      GetVolume());
    xmlInfo->Set("Pitch",       GetPitch());
    xmlInfo->Set("Range",       GetRange());
    xmlInfo->Set("Looping",     IsLooping());
    xmlInfo->Set("PlayOnStart", IsPlayOnStart());
}
