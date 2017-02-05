#include "AudioClip.h"

#include <QThread>

#include "Debug.h"
#include "XMLNode.h"
#include "AudioManager.h"

AudioClip::AudioClip()
{
}

AudioClip::~AudioClip()
{
    Free();
}

const String AudioClip::GetFileExtensionStatic()
{
    return "baudioclip";
}

const String AudioClip::GetFileExtension()
{
    return AudioClip::GetFileExtension();
}

void AudioClip::LoadFromFile(const String &filepath)
{
    Free();

    alGetError();

    // Fill buffer from PCM
    m_alBufferId = alutCreateBufferFromFile(filepath.ToCString());
}

void AudioClip::Play()
{
    ASSERT(IsLoaded());
    AudioPlayProperties props;
    AudioManager::PlayAudioClip(this, props);
}

void AudioClip::Pause()
{
}

void AudioClip::Stop()
{
}

bool AudioClip::IsLoaded() const
{
    return m_alBufferId != 0;
}

void AudioClip::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    m_filepath = xmlInfo->GetFilepath("AudioFilepath");
    LoadFromFile(m_filepath);
}

void AudioClip::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("AudioClip");
    xmlInfo->SetFilepath("AudioFilepath", m_filepath, "ogg wav");
}

ALuint AudioClip::GetALBufferId() const
{
    return m_alBufferId;
}

void AudioClip::Free()
{
    if ( IsLoaded() )
    {
        alDeleteBuffers(1, &m_alBufferId);
    }
}

