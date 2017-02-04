#include "AudioPlayerRunnable.h"

#include <QThread>

#include "Debug.h"
#include "AudioClip.h"

AudioPlayerRunnable::AudioPlayerRunnable(AudioClip *clip, float delaySeconds)
{
    m_audioClip = clip;
    m_delaySeconds = delaySeconds;
}

void AudioPlayerRunnable::run()
{
    ASSERT(m_audioClip->IsLoaded());

    if (m_delaySeconds > 0.0f)
    {
        QThread::currentThread()->msleep(m_delaySeconds * 1000);
    }

    alSourcePlay( m_audioClip->GetALSourceId() );
    while (m_audioClip->IsPlaying())
    {
        QThread::currentThread()->msleep(100);
    }
}
