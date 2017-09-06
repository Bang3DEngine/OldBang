#include "Bang/AudioClip.h"

#include <sndfile.h>

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"

USING_NAMESPACE_BANG

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

void AudioClip::Import(const Path &soundFilepath)
{
    if (!soundFilepath.Exists() || !soundFilepath.IsFile()) { return; }

    if (soundFilepath.HasExtension("ogg"))
    {
        Debug_Error("OGG audio file format for " << soundFilepath.GetAbsolute()
                    << " not supported.");
        return;
    }

    AudioManager::ClearALErrors();
    AudioManager::DettachSourcesFromAudioClip(this);
    alDeleteBuffers(1, &m_alBufferId);
    alGenBuffers(1, &m_alBufferId);
    AudioManager::CheckALError();

    AudioManager::ClearALErrors();

    SF_INFO soundInfo;
    SNDFILE *soundFile = sf_open(soundFilepath.GetAbsolute().ToCString(),
                                 SFM_READ,
                                 &soundInfo);

    constexpr int bufferSize = 4096;
    Array<short> buffer(bufferSize);
    Array<short> readData;
    while (true)
    {
        size_t readSize = sf_read_short(soundFile,
                                        &buffer.Front(),
                                        bufferSize);
        if (readSize <= 0) { break; }
        readData.PushBack(buffer.begin(), buffer.begin() + readSize);
    }
    AudioManager::CheckALError();

    AudioManager::ClearALErrors();
    bool hasError = AudioManager::CheckALError();
    if (!hasError)
    {
        alBufferData(m_alBufferId,
                     soundInfo.channels == 1? AL_FORMAT_MONO16 :
                                              AL_FORMAT_STEREO16,
                     &readData.Front(),
                     readData.Size() * sizeof(short),
                     soundInfo.samplerate);
        m_soundFilepath = soundFilepath;

        hasError = AudioManager::CheckALError();
    }
    else { m_soundFilepath = Path::Empty; }

    return;
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

void AudioClip::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void AudioClip::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}
