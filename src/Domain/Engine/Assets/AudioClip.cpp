#include "AudioClip.h"

#include <QThread>

#include "Debug.h"
#include "XMLNode.h"
#include "AudioSource.h"
#include "AudioManager.h"

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

const String AudioClip::GetFileExtensionStatic()
{
    return "baudioclip";
}

const String AudioClip::GetFileExtension()
{
    return AudioClip::GetFileExtension();
}

bool AudioClip::LoadFromFile(const String &filepath)
{
    ASSERT(!filepath.Empty(), "", false);
    ASSERT(filepath != m_audioFileFilepath, "", true);
    if (filepath.EndsWith(".ogg"))
    {
        Debug_Error("OGG audio file format for " << filepath << " not supported.");
        return false;
    }

    AudioManager::ClearALErrors();

    // First unbind all sources from the buffer
    // OpenAL does not let changing a buffer dynamically
    // if one or more sources are attached to it
    for (AudioSource *as : m_audioSourcesUsingThis)
    {
        as->Stop();
        as->SetAudioClipNoDettachAttach(nullptr);
    }

    ALsizei size; ALfloat freq; ALenum format;
    ALvoid *data =
         alutLoadMemoryFromFile(filepath.ToCString(),
                                &format, &size, &freq);

    bool hasError = AudioManager::CheckALError();
    if (!hasError)
    {
        alBufferData(m_alBufferId, format, data, size, freq);
        free(data);

        hasError = AudioManager::CheckALError();
    }

    // Rebind all previously dettached sources
    for (AudioSource *as : m_audioSourcesUsingThis)
    {
        as->SetAudioClipNoDettachAttach(hasError ? nullptr : this);
    }

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

bool AudioClip::IsLoaded() const
{
    return m_alBufferId != 0;
}

const String &AudioClip::GetAudioFilepath() const
{
    return m_audioFileFilepath;
}

void AudioClip::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);

    m_audioFileFilepath = xmlInfo->GetFilepath("AudioFilepath");
    LoadFromFile( m_audioFileFilepath );
}

void AudioClip::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("AudioClip");
    xmlInfo->SetFilepath("AudioFilepath", m_audioFileFilepath, "ogg wav");
}

void AudioClip::OnAudioSourceAttached(AudioSource *as)
{
    m_audioSourcesUsingThis.PushBack(as);
}

void AudioClip::OnAudioSourceDettached(AudioSource *as)
{
    m_audioSourcesUsingThis.RemoveAll(as);
}

ALuint AudioClip::GetALBufferId() const
{
    return m_alBufferId;
}

