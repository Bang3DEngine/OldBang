#include "AudioSource.h"

#include "XMLNode.h"
#include "AudioClip.h"
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
    as->SetAudioClip( m_audioClip );
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
    m_velocity = xmlInfo->GetFloat("Velocity");
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
    xmlInfo->SetFloat("Volume", m_volume);
    xmlInfo->SetFloat("Velocity", m_velocity);
    xmlInfo->SetFloat("Pitch", m_pitch);
    xmlInfo->SetBool("Looping", m_looping);
}

void AudioSource::Play(float delaySeconds)
{
    ASSERT(m_audioClip);
    m_audioClip->Play(delaySeconds);
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
