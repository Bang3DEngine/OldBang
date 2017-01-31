#include "AudioClip.h"

#include <QThread>

#include "Debug.h"
#include "XMLNode.h"

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

    // Create sound source (use buffer to fill source)
    alGenSources(1, &m_alSourceId);
    alSourcei(m_alSourceId, AL_BUFFER, m_alBufferId);
}

void AudioClip::Play()
{
    ASSERT(IsLoaded());
    alSourcePlay(m_alSourceId);

    while (IsPlaying())
    {
        QThread::currentThread()->msleep(100);
    }
}

void AudioClip::Pause()
{
    ASSERT(IsLoaded());
    ASSERT(IsPlaying());
    alSourcePause(m_alSourceId);
}

void AudioClip::Stop()
{
    ASSERT(IsLoaded());
    alSourceStop(m_alSourceId);
}

bool AudioClip::IsLoaded() const
{
    return m_alSourceId != 0;
}

bool AudioClip::IsPlaying() const
{
    return IsLoaded() && GetState() == State::Playing;
}

bool AudioClip::IsPaused() const
{
    return IsLoaded() && GetState() == State::Paused;
}

bool AudioClip::IsStopped() const
{
    return IsLoaded() && GetState() == State::Stopped;
}

AudioClip::State AudioClip::GetState() const
{
    ALint state;
    alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &state);
    return static_cast<State>(state);
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

void AudioClip::Free()
{
    if ( IsLoaded() )
    {
        alDeleteSources(1, &m_alSourceId);
        alDeleteBuffers(1, &m_alBufferId);
    }
}

