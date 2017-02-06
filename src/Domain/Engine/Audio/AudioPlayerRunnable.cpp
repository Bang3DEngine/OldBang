#include "AudioPlayerRunnable.h"

#include <QThread>

#include "Debug.h"
#include "AudioClip.h"
#include "AudioManager.h"

AudioPlayerRunnable::AudioPlayerRunnable(AudioClip *clip,
                                         const AudioPlayProperties &properties)
{
    m_audioClip = clip;
    m_properties = properties;
}

void AudioPlayerRunnable::run()
{
    ASSERT(m_audioClip->IsLoaded());

    if (m_properties.delayInSeconds > 0.0f)
    {
        QThread::currentThread()->msleep(m_properties.delayInSeconds * 1000);
    }

    AudioManager::CheckALError();
    alSourcePlay(m_properties.alSourceId);

    ALint state;
    do
    {
        alGetSourcei(m_properties.alSourceId, AL_SOURCE_STATE, &state);
        QThread::currentThread()->msleep(100);
    }
    while ( state == AL_PLAYING );
}
