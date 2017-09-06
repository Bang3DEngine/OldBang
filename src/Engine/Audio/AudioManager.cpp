#include "Bang/AudioManager.h"

#include "Bang/Path.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Vector3.h"
#include "Bang/Resources.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/ALAudioSource.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioPlayerRunnable.h"

USING_NAMESPACE_BANG

AudioManager::AudioManager()
{
    InitAL();
    m_threadPool.SetMaxThreadCount(256);
}

AudioManager::~AudioManager()
{
    for (AudioPlayerRunnable *audioPlayer : m_currentAudioPlayers)
    {
        ALuint sourceId = audioPlayer->GetALAudioSource()->GetALSourceId();
        alDeleteSources(1, &sourceId);
    }
    StopAllSounds();

    Array<AudioClip*> allAudioClips = Resources::GetAll<AudioClip>();
    for (AudioClip *ac : allAudioClips) { Resources::UnLoad(ac, true); }

    alcDestroyContext(m_alContext);
    alcCloseDevice(m_alDevice);
}

bool AudioManager::InitAL()
{
    m_alDevice = alcOpenDevice(NULL);
    if(!m_alDevice) { Debug_Error("Could not start OpenAL Device");
                      return false; }

    m_alContext = alcCreateContext(m_alDevice, NULL);
    if(!m_alContext) { Debug_Error("Could not start OpenAL Context");
                       return false; }

    alcMakeContextCurrent(m_alContext);
    ClearALErrors();

    return true;
}

void AudioManager::Play(AudioClip *audioClip,
                        ALAudioSource *aas,
                        float delay)
{
    ENSURE(audioClip);
    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip, aas, delay);
    AudioManager *am = AudioManager::GetInstance();
    bool started = am->m_threadPool.TryStart(player);
    if (started)
    {
        MutexLocker m(&am->m_mutex_currentAudios);
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
    AudioClip *audioClip = Resources::Load<AudioClip>(audioClipFilepath);
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

    MutexLocker m(&am->m_mutex_currentAudios);
    am->m_currentAudioPlayers.Clear();
}

void AudioManager::OnAudioFinishedPlaying(AudioPlayerRunnable *audioPlayer)
{
    MutexLocker m(&m_mutex_currentAudios);
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
        Debug_Error("OpenAL error(" << error << "): " << errorStr);
    }
    return hasError;
}

AudioManager *AudioManager::GetInstance()
{
    return Application::GetInstance()->GetAudioManager();
}
