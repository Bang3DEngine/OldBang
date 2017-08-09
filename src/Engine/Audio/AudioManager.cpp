#include "Bang/AudioManager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Path.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Vector3.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/ALAudioSource.h"
#include "Bang/AssetsManager.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioPlayerRunnable.h"

AudioManager::AudioManager()
{
    alutInit(0, NULL);
    m_threadPool.setMaxThreadCount(256);
}

AudioManager::~AudioManager()
{
    for (AudioPlayerRunnable *ap : m_currentAudioPlayers) { ap->Stop(); }
    alutExit();
}

void AudioManager::Play(AudioClip *audioClip,
                        ALAudioSource *aas,
                        float delay)
{
    ENSURE(audioClip);
    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip, aas, delay);
    AudioManager *am = AudioManager::GetInstance();
    bool started = am->m_threadPool.tryStart(player);
    if (started)
    {
        QMutexLocker m(&am->m_mutex_currentAudios);
        am->m_currentAudioPlayers.Add(player);
    }
}

void AudioManager::Play(AudioClip *audioClip,
                        const AudioParams &params,
                        float delay)
{
    ENSURE(audioClip);
    ALAudioSource *aas = new ALAudioSource();
    aas->SetALBufferId(audioClip->GetALBufferId());
    aas->SetParams(params);
    aas->m_autoDelete = true;
    AudioManager::Play(audioClip, aas, delay);
}

void AudioManager::Play(const Path &audioClipFilepath,
                        const AudioParams &params,
                        float delay)
{
    AudioClip *audioClip = AssetsManager::Load<AudioClip>(audioClipFilepath);
    AudioManager::Play(audioClip, params, delay);
}

void AudioManager::PauseAllSounds()
{
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *audioPlayer : am->m_currentAudioPlayers)
    {
        audioPlayer->Pause();
    }
}

void AudioManager::ResumeAllSounds()
{
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *audioPlayer : am->m_currentAudioPlayers)
    {
        audioPlayer->Resume();
    }
}

void AudioManager::StopAllSounds()
{
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *audioPlayer : am->m_currentAudioPlayers)
    {
        audioPlayer->Stop();
    }

    QMutexLocker m(&am->m_mutex_currentAudios);
    am->m_currentAudioPlayers.Clear();
}

void AudioManager::OnAudioFinishedPlaying(AudioPlayerRunnable *audioPlayer)
{
    QMutexLocker m(&m_mutex_currentAudios);
    m_currentAudioPlayers.Remove(audioPlayer);
}

void AudioManager::DettachSourcesFromAudioClip(AudioClip *ac)
{
    // Dettach all audioSources using this AudioClip.
    // Otherwise OpenAL throws error.
    AudioManager *am = AudioManager::GetInstance();
    for (AudioPlayerRunnable *ap : am->m_currentAudioPlayers)
    {
        if (ap->GetAudioClip() == ac)
        {
            ap->Stop();
            ap->GetALAudioSource()->SetALBufferId(0);
        }
    }
}

void AudioManager::ClearALErrors()
{
    alGetError();
}

bool AudioManager::CheckALError()
{
    ALenum error = alGetError();
    bool hasError = (error != AL_NO_ERROR);
    if (hasError)
    {
        const char *errorStr = alGetString(error);
        Debug_Error("OpenAL error: " << errorStr);
    }
    return hasError;
}

AudioManager *AudioManager::GetInstance()
{
    return Application::GetInstance()->GetAudioManager();
}
