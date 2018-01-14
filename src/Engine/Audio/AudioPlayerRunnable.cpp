#include "Bang/AudioPlayerRunnable.h"

#include "Bang/AudioClip.h"
#include "Bang/AudioManager.h"
#include "Bang/ALAudioSource.h"

USING_NAMESPACE_BANG

AudioPlayerRunnable::AudioPlayerRunnable(AudioClip *clip,
                                         ALAudioSource *alAudioSource,
                                         float delayInSeconds)
{
    p_audioClip = clip;
    p_alAudioSource = alAudioSource;
    m_delayInSeconds = delayInSeconds;
    SetAutoDelete(true);
}

AudioPlayerRunnable::~AudioPlayerRunnable()
{
    AudioManager::GetInstance()->OnAudioFinishedPlaying(this);
    EventEmitter<IDestroyListener>::PropagateToListeners(
                          &IDestroyListener::OnDestroyed, this);
}

void AudioPlayerRunnable::Resume() { p_alAudioSource->Play(); }
void AudioPlayerRunnable::Pause() { p_alAudioSource->Pause(); }
void AudioPlayerRunnable::Stop()
{
    m_forceExit = true;
    p_alAudioSource->Stop();
}
AudioClip *AudioPlayerRunnable::GetAudioClip() const { return p_audioClip; }
ALAudioSource *AudioPlayerRunnable::GetALAudioSource() const
{
    return p_alAudioSource;
}

void AudioPlayerRunnable::Run()
{
    ENSURE(p_audioClip->IsLoaded());

    if (m_delayInSeconds > 0.0f) // Wait delay
    {
        Thread::SleepCurrentThread(m_delayInSeconds);
    }

    p_alAudioSource->Play(); // Play and wait until source is stopped
    do
    {
        Thread::SleepCurrentThread(0.3f);
    }
    while ( !m_forceExit && !p_alAudioSource->IsStopped() );
}
