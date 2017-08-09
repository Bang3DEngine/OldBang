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
    AudioManager::GetInstance()->OnAudioFinishedPlaying(this);
    if (m_alAudioSource->m_autoDelete) { delete m_alAudioSource; }
}

void AudioPlayerRunnable::Resume() { m_alAudioSource->Play(); }
void AudioPlayerRunnable::Pause() { m_alAudioSource->Pause(); }
void AudioPlayerRunnable::Stop() { m_alAudioSource->Stop(); }
AudioClip *AudioPlayerRunnable::GetAudioClip() const { return m_audioClip; }
ALAudioSource *AudioPlayerRunnable::GetALAudioSource() const
{
    return m_alAudioSource;
}

void AudioPlayerRunnable::run()
{
    ENSURE(m_audioClip->IsLoaded());

    if (m_delayInSeconds > 0.0f) // Wait delay
    {
        QThread::currentThread()->msleep(m_delayInSeconds * 1000);
    }

    AudioManager::ClearALErrors();
    AudioManager::CheckALError();

    m_alAudioSource->Play(); // Play and wait until source is stopped
    do { QThread::currentThread()->msleep(1000); }
    while ( !m_alAudioSource->IsStopped() );
}
