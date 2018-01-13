#include "Bang/AudioClip.h"

#include <sndfile.h>

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/Resources.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"

USING_NAMESPACE_BANG

AudioClip::AudioClip()
{
    AudioManager::ClearALErrors();
    alGenBuffers(1, &m_alBufferId);
    AudioManager::CheckALError();
}

AudioClip::~AudioClip()
{
    FreeBuffer();
}

void AudioClip::Import(const Path &soundFilepath)
{
    if (!soundFilepath.Exists() || !soundFilepath.IsFile()) { return; }

    AudioManager::ClearALErrors();
    AudioManager::DettachSourcesFromAudioClip(this);
    FreeBuffer();
    alGenBuffers(1, &m_alBufferId);
    AudioManager::CheckALError();


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

    AudioManager::ClearALErrors();
    alBufferData(m_alBufferId,
                 soundInfo.channels == 1? AL_FORMAT_MONO16 :
                                          AL_FORMAT_STEREO16,
                 &readData.Front(),
                 readData.Size() * sizeof(short),
                 soundInfo.samplerate);
    bool hasError = AudioManager::CheckALError();

    if(!hasError) { m_soundFilepath = soundFilepath; }
    else { m_soundFilepath = Path::Empty; }
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

void AudioClip::FreeBuffer()
{
    if ( IsLoaded() )
    {
        alDeleteBuffers(1, &m_alBufferId);
        m_alBufferId = 0;
    }
}

void AudioClip::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void AudioClip::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}
