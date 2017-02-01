#include "AudioPlayerRunnable.h"

#include <QThread>

#include "Debug.h"
#include "AudioClip.h"

AudioPlayerRunnable::AudioPlayerRunnable(AudioClip *clip)
{
    m_audioClip = clip;
}

void AudioPlayerRunnable::run()
{
    ASSERT(m_audioClip->IsLoaded());

    alSourcePlay( m_audioClip->GetALSourceId() );
    while (m_audioClip->IsPlaying())
    {
        QThread::currentThread()->msleep(100);
    }
}
