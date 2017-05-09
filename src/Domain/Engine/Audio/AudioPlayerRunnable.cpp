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
    setAutoDelete(true);
}

void AudioPlayerRunnable::Resume()
{
    m_paused = false;
}

void AudioPlayerRunnable::Pause()
{
    m_paused = true;
}

void AudioPlayerRunnable::Stop()
{
    m_stopped = true;
}

void AudioPlayerRunnable::OnAudioManagerDelete()
{
    m_exited = true;
}

void AudioPlayerRunnable::run()
{
    ENSURE(m_audioClip->IsLoaded());

    if (m_delayInSeconds > 0.0f)
    {
        QThread::currentThread()->msleep(m_delayInSeconds * 1000);
    }

    AudioManager::ClearALErrors();
    AudioManager::CheckALError();

    ALint state;
    do
    {
        if (m_stopped || m_exited) { alSourceStop (m_alSourceId); }
        if (m_paused)  { alSourcePause(m_alSourceId); }

        alGetSourcei(m_alSourceId, AL_SOURCE_STATE, &state);
        if (!m_paused && state != AL_PLAYING)
        {
            alSourcePlay(m_alSourceId); // Resume
        }

        QThread::currentThread()->msleep(100);
    }
    while ( state != AL_STOPPED && !m_exited );

    if (!m_exited)
    {
        AudioManager::GetInstance()->OnAudioFinishedPlaying(this);
    }
}
