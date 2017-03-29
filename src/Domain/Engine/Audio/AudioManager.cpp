#include "Bang/AudioManager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <QThreadPool>
#include "Bang/WinUndef.h"

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/AudioListener.h"
#include "Bang/AudioPlayerRunnable.h"

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
    audioManager->m_threadPool.tryStart(player);
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


