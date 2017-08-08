#include "Bang/AudioPlayerRunnable.h"

#include <QThread>

#include "Bang/AudioClip.h"
#include "Bang/AudioManager.h"
#include "Bang/ALAudioSource.h"

AudioPlayerRunnable::AudioPlayerRunnable(AudioClip *clip,
                                         ALAudioSource *alAudioSource,
                                         float delayInSeconds)
{
    m_audioClip = clip;
    m_alAudioSource = alAudioSource;
    m_delayInSeconds = delayInSeconds;
    setAutoDelete(true);
}

AudioPlayerRunnable::~AudioPlayerRunnable()
{
    if (m_alAudioSource->m_autoDelete) { delete m_alAudioSource; }
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

void AudioPlayerRunnable::Destroy()
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

    m_alAudioSource->Play();
    m_alAudioSource->Stop(); // So we can assume initial state is AL_STOPPED
    do
    {
        if (m_stopped || m_exited) { m_alAudioSource->Stop(); break; }
        if (m_paused)  { m_alAudioSource->Pause(); }

        if (!m_paused && !m_alAudioSource->IsPlaying())
        {
            m_alAudioSource->Play(); // Resume
        }

        QThread::currentThread()->msleep(100);
    }
    while ( !m_alAudioSource->IsStopped() && !m_exited );

    if (!m_exited)
    {
        AudioManager::GetInstance()->OnAudioFinishedPlaying(this);
    }
}
