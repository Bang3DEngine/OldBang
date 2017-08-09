#include "Bang/AudioClip.h"

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"

AudioClip::AudioClip()
{
    alGenBuffers(1, &m_alBufferId);
}

AudioClip::~AudioClip()
{
    if ( IsLoaded() )
    {
        alDeleteBuffers(1, &m_alBufferId);
    }
}

bool AudioClip::LoadFromSoundFile(const Path &filepath)
{
    if (!filepath.Exists() || !filepath.IsFile()) { return false; }

    if (filepath.HasExtension("ogg"))
    {
        Debug_Error("OGG audio file format for " << filepath.GetAbsolute()
                    << " not supported.");
        return false;
    }

    AudioManager::ClearALErrors();
    AudioManager::DettachSourcesFromAudioClip(this);
    alDeleteBuffers(1, &m_alBufferId);
    alGenBuffers(1, &m_alBufferId);
    AudioManager::CheckALError();

    AudioManager::ClearALErrors();
    ALsizei size; ALfloat freq; ALenum format;
    ALvoid *data = alutLoadMemoryFromFile(filepath.GetAbsolute().ToCString(),
                                          &format, &size, &freq);
    AudioManager::CheckALError();

    AudioManager::ClearALErrors();
    bool hasError = AudioManager::CheckALError();
    if (!hasError)
    {
        alBufferData(m_alBufferId, format, data, size, freq);
        m_soundFilepath = filepath;
        free(data);

        hasError = AudioManager::CheckALError();
    }
    else { m_soundFilepath = Path(); }

    return !hasError;
}

int AudioClip::GetChannels() const
{
    if (!IsLoaded()) { return 0; }

    int channels;
    alGetBufferi(m_alBufferId, AL_CHANNELS, &channels);
    return channels;
}

int AudioClip::GetBufferSize() const
{
    if (!IsLoaded()) { return 0; }

    int bSize;
    alGetBufferi(m_alBufferId, AL_SIZE, &bSize);
    return bSize;
}

int AudioClip::GetBitDepth() const
{
    if (!IsLoaded()) { return 0; }

    int bitDepth;
    alGetBufferi(m_alBufferId, AL_BITS, &bitDepth);
    return bitDepth;
}

int AudioClip::GetFrequency() const
{
    if (!IsLoaded()) { return 0; }

    int freq;
    alGetBufferi(m_alBufferId, AL_FREQUENCY, &freq);
    return freq;
}

float AudioClip::GetLength() const
{
    if (!IsLoaded()) { return 0.0f; }

    const int bitChannels = (GetBitDepth() / 8) * GetChannels();
    return (float(GetBufferSize()) / bitChannels) / GetFrequency();
}

ALuint AudioClip::GetALBufferId() const
{
    return m_alBufferId;
}

bool AudioClip::IsLoaded() const
{
    return m_alBufferId != 0;
}

const Path &AudioClip::GetSoundFilepath() const
{
    return m_soundFilepath;
}

void AudioClip::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);
    m_soundFilepath = xmlInfo.Get<Path>("AudioFilepath");
    LoadFromSoundFile( m_soundFilepath );
}

void AudioClip::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);
    xmlInfo->Set("AudioFilepath", m_soundFilepath);
}
