#include "AudioPlayerRunnable.h"

#include <QThread>

#include "Debug.h"
#include "AudioClip.h"

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


    ALint sourceId = m_audioClip->GetALSourceId();
    alSourcef(sourceId,  AL_PITCH,    m_properties.pitch);
    alSourcef(sourceId,  AL_GAIN,     m_properties.volume);
    alSource3f(sourceId, AL_POSITION, m_properties.sourcePosition.x,
                                      m_properties.sourcePosition.y,
                                      m_properties.sourcePosition.z);
    alSource3f(sourceId, AL_VELOCITY, m_properties.sourceVelocity.x,
                                      m_properties.sourceVelocity.y,
                                      m_properties.sourceVelocity.z);
    alSourcei(sourceId,  AL_LOOPING,  m_properties.looping);

    alSourcePlay(sourceId);
    while (m_audioClip->IsPlaying())
    {
        QThread::currentThread()->msleep(100);
    }
}
