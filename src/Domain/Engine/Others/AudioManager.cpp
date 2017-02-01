#include "AudioManager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <QThreadPool>

#include "Debug.h"
#include "AudioClip.h"
#include "Application.h"
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

void AudioManager::PlayAudioClip(AudioClip *audioClip)
{
    ASSERT(audioClip);
    AudioManager *audioManager = AudioManager::GetInstance();

    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip);
    bool hasBeenAbleToPlay = audioManager->m_threadPool.tryStart(player);
}

AudioManager *AudioManager::GetInstance()
{
    return Application::GetInstance()->GetAudioManager();
}


