#include "AudioSource.h"

#include "XMLNode.h"
#include "AudioClip.h"
#include "Transform.h"
#include "ICloneable.h"
#include "AssetsManager.h"

AudioSource::AudioSource()
{
}

AudioSource::~AudioSource()
{
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
    m_volume   = xmlInfo->GetFloat("Volume");
    m_pitch    = xmlInfo->GetFloat("Pitch");
    m_looping  = xmlInfo->GetBool("Looping");
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

    Debug_Log("IsPlaying? " << IsPlaying());
    AudioSource *noConstThis = const_cast<AudioSource*>(this);
    if (IsPlaying())
    {
        xmlInfo->SetButton("Stop", noConstThis);
    }
    else
    {
        xmlInfo->SetButton("Play", noConstThis);
    }
}

void AudioSource::Play(float delaySeconds)
{
    ASSERT(m_audioClip);

    AudioPlayProperties props = GetAudioPlayProperties();
    props.delayInSeconds = delaySeconds;
    m_audioClip->Play( props );
}

void AudioSource::Pause()
{
    ASSERT(m_audioClip);
    m_audioClip->Pause();
}

void AudioSource::Stop()
{
    ASSERT(m_audioClip);
    m_audioClip->Stop();
}

AudioClip *AudioSource::GetAudioClip() const
{
    return m_audioClip;
}

void AudioSource::SetAudioClip(AudioClip *audioClip)
{
    m_audioClip = audioClip;
}

AudioPlayProperties AudioSource::GetAudioPlayProperties() const
{
    AudioPlayProperties props;
    props.delayInSeconds = 0.0f;
    props.pitch          = m_pitch;
    props.volume         = m_volume;
    props.looping        = m_looping;
    props.sourcePosition = transform->GetPosition();
    props.sourceVelocity = Vector3::Zero;
    return props;
}

void AudioSource::SetVolume(float volume)
{
    m_volume = volume;
}

void AudioSource::SetPitch(float pitch)
{
    m_pitch = pitch;
}

void AudioSource::SetLooping(bool looping)
{
    m_looping = looping;
}

bool AudioSource::IsPlaying() const
{
    AudioClip *ac = GetAudioClip();
    return ac ? ac->IsPlaying() : false;
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
