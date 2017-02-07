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

void AudioManager::PlayAudioClip(AudioClip *audioClip, int alSourceId,
                                 float delayInSeconds)
{
    ASSERT(audioClip);
    AudioManager *audioManager = AudioManager::GetInstance();

    AudioListener *listener = SceneManager::GetActiveScene()->
            GetComponentInChildren<AudioListener>();
    if (!listener)
    {
        Debug_Warn ("The scene does not contain an AudioListener");
    }

    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip, alSourceId,
                                                          delayInSeconds);
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


