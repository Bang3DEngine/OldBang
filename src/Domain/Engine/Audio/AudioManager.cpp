#include "AudioManager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <QThreadPool>

#include "Scene.h"
#include "Debug.h"
#include "Camera.h"
#include "Transform.h"
#include "AudioClip.h"
#include "GameObject.h"
#include "Application.h"
#include "SceneManager.h"
#include "AudioListener.h"
#include "AudioPlayProperties.h"
#include "AudioPlayerRunnable.h"

AudioManager::AudioManager()
{
    alutInit(0, NULL);
    m_threadPool.setMaxThreadCount(256);
}

AudioManager::~AudioManager()
{
    alutExit();
}

void AudioManager::PlayAudioClip(AudioClip *audioClip,
                                 const AudioPlayProperties &audioPlayProperties)
{
    ASSERT(audioClip);
    AudioManager *audioManager = AudioManager::GetInstance();

    AudioPlayProperties playProps = audioPlayProperties;
    AudioListener *listener = SceneManager::GetActiveScene()->
            GetComponentInChildren<AudioListener>();
    if (listener)
    {
        playProps.listenerPosition    = listener->transform->GetPosition();
        playProps.listenerOrientation = listener->transform->GetEuler();
        playProps.listenerVelocity    = Vector3::Zero;
    }
    else
    {
        Debug_Warn ("The scene does not contain an AudioListener");
    }

    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip, playProps);
    bool hasBeenAbleToPlay = audioManager->m_threadPool.tryStart(player);
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


