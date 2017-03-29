#include "Bang/AudioPlayerRunnable.h"

#include <QThread>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioManager.h"

AudioPlayerRunnable::AudioPlayerRunnable(AudioClip *clip,
                                         int alSourceId,
                                         float delayInSeconds)
{
    m_audioClip = clip;
    m_alSourceId = alSourceId;
    m_delayInSeconds = delayInSeconds;
}

void AudioPlayerRunnable::run()
{
    ASSERT(m_audioClip->IsLoaded());

    if (m_delayInSeconds > 0.0f)
    {
        QThread::currentThread()->msleep(m_delayInSeconds * 1000);
    }

    AudioManager::CheckALError();
    alSourcePlay(m_alSourceId);

    ALint state;
    do
    {
        alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &state);
        QThread::currentThread()->msleep(100);
    }
    while ( state == AL_PLAYING );
}
