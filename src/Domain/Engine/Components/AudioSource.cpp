#include "AudioSource.h"

#include "XMLNode.h"
#include "AudioClip.h"
#include "Transform.h"
#include "AudioClip.h"
#include "ICloneable.h"
#include "AudioManager.h"
#include "AssetsManager.h"

AudioSource::AudioSource()
{
    alGenSources(1, &m_alSourceId);
}

AudioSource::~AudioSource()
{
    Stop();

    if (m_audioClip)
    {
        m_audioClip->OnAudioSourceDettached(this);
    }

    alDeleteSources(1, &m_alSourceId);
}

String AudioSource::GetName() const
{
    return "AudioSource";
}

void AudioSource::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    AudioSource *as = static_cast<AudioSource*>(clone);
    as->SetAudioClip( GetAudioClip() );
    as->SetVolume( GetVolume()  );
    as->SetPitch( GetPitch() );
    as->SetLooping( IsLooping() );
}

ICloneable *AudioSource::Clone() const
{
    AudioSource *as = new AudioSource();
    CloneInto(as);
    return as;
}

#ifdef BANG_EDITOR
void AudioSource::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void AudioSource::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}
#endif

void AudioSource::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);

    String audioClipFilepath = m_audioClip ? m_audioClip->GetFilepath() : "";
    String newAudioClipFilepath = xmlInfo->GetFilepath("AudioClip");
    if (audioClipFilepath != newAudioClipFilepath)
    {
        SetAudioClip( AssetsManager::Load<AudioClip>(newAudioClipFilepath) );
    }
    SetVolume(xmlInfo->GetFloat("Volume"));
    SetPitch(xmlInfo->GetFloat("Pitch"));
    SetLooping(xmlInfo->GetBool("Looping"));
}

void AudioSource::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName( GetName() );

    String audioClipFilepath = m_audioClip ? m_audioClip->GetFilepath() : "";
    xmlInfo->SetFilepath("AudioClip", audioClipFilepath,
                         AudioClip::GetFileExtensionStatic());
    xmlInfo->SetFloat("Volume",   m_volume);
    xmlInfo->SetFloat("Pitch",    m_pitch);
    xmlInfo->SetBool("Looping",   m_looping);

    AudioSource *noConstThis = const_cast<AudioSource*>(this);
    if (IsPlaying())
    {
        xmlInfo->SetButton("Stop", noConstThis, {});
        xmlInfo->SetButton("Play", noConstThis, {XMLProperty::Hidden});
    }
    else
    {

        xmlInfo->SetButton("Stop", noConstThis, {XMLProperty::Hidden});
        xmlInfo->SetButton("Play", noConstThis, {});
    }
}

void AudioSource::Play(float delaySeconds)
{
    ASSERT(m_audioClip);

    AudioPlayProperties props = GetAudioPlayProperties();
    props.delayInSeconds = delaySeconds;
    AudioManager::PlayAudioClip(m_audioClip, props);
}

void AudioSource::Pause()
{
    ASSERT(m_audioClip);
    alSourcePause(m_alSourceId);
}

void AudioSource::Stop()
{
    ASSERT(m_audioClip);
    alSourceStop(m_alSourceId);
}

AudioClip *AudioSource::GetAudioClip() const
{
    return m_audioClip;
}

void AudioSource::SetAudioClip(AudioClip *audioClip)
{
    if (m_audioClip)
    {
        m_audioClip->OnAudioSourceDettached(this);
    }

    SetAudioClipNoDettachAttach(audioClip);

    if (m_audioClip)
    {
        m_audioClip->OnAudioSourceAttached(this);
    }
}

AudioPlayProperties AudioSource::GetAudioPlayProperties() const
{
    AudioPlayProperties props;
    props.delayInSeconds = 0.0f;
    props.pitch          = m_pitch;
    props.volume         = m_volume;
    props.looping        = m_looping;
    props.sourcePosition = gameObject ? transform->GetPosition() : Vector3::Zero;
    props.sourceVelocity = Vector3::Zero;
    props.alSourceId     = GetALSourceId();
    return props;
}

void AudioSource::SetVolume(float volume)
{
    m_volume = volume;
    alSourcef(GetALSourceId(), AL_GAIN, m_volume);
}

void AudioSource::SetPitch(float pitch)
{
    m_pitch = pitch;
    alSourcef(GetALSourceId(), AL_PITCH, m_pitch);
}

void AudioSource::SetLooping(bool looping)
{
    m_looping = looping;
    alSourcef(GetALSourceId(), AL_LOOPING, m_looping);
}

bool AudioSource::IsPlaying() const
{
    return GetState() == State::Playing;
}

bool AudioSource::IsPaused() const
{
    return GetState() == State::Paused;
}

bool AudioSource::IsStopped() const
{
    return GetState() == State::Stopped;
}

AudioSource::State AudioSource::GetState() const
{
    ALint state;
    alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &state);
    return static_cast<State>(state);
}

float AudioSource::GetPlayProgress() const
{
    ASSERT(m_audioClip, "", 0.0f);

    float secondsOffset;
    alGetSourcef(m_alSourceId, AL_SEC_OFFSET, &secondsOffset);
    return secondsOffset / m_audioClip->GetLength();
}

float AudioSource::GetVolume() const
{
    return m_volume;
}

float AudioSource::GetPitch() const
{
    return m_pitch;
}

bool AudioSource::IsLooping() const
{
    return m_looping;
}

void AudioSource::OnButtonClicked(const String &attrName)
{
    if (IsPlaying())
    {
        Stop();
    }
    else
    {
        Play();
    }
}

ALuint AudioSource::GetALSourceId() const
{
    return m_alSourceId;
}

void AudioSource::SetAudioClipNoDettachAttach(AudioClip *audioClip)
{
    m_audioClip = audioClip;
    alSourcei(m_alSourceId, AL_BUFFER,
              audioClip ? m_audioClip->GetALBufferId() : 0);
}
