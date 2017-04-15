#include "Bang/AudioManager.h"

#include <QThreadPool>
#include "Bang/WinUndef.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/AudioClip.h"
#include "Bang/GameObject.h"
#include "Bang/AudioSource.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/AudioListener.h"
#include "Bang/AssetsManager.h"
#include "Bang/AudioPlayerRunnable.h"
#include "Bang/AnonymousAudioPlayer.h"

AudioManager::AudioManager()
{
    alutInit(0, NULL);
    m_threadPool.setMaxThreadCount(256);
    m_anonymousAudioPlayer = new AnonymousAudioPlayer();
}

AudioManager::~AudioManager()
{
    delete m_anonymousAudioPlayer;
    alutExit();
}

AnonymousAudioPlayer *AudioManager::GetAnonymousAudioPlayer() const
{
    return GetInstance()->m_anonymousAudioPlayer;
}

void AudioManager::PlayAudioClip(AudioClip *audioClip,
                                 int alSourceId,
                                 float delay)
{
    ENSURE(audioClip);

    AudioListener *listener = SceneManager::GetActiveScene()->
            GetComponentInChildren<AudioListener>();
    if (!listener)
    {
        Debug_Warn ("The scene does not contain an AudioListener");
    }

    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip, alSourceId,
                                                          delay);
    AudioManager::GetInstance()->m_threadPool.tryStart(player);
}

void AudioManager::PlayAudioClip(const String &audioClipFilepath,
                                 const Vector3& position,
                                 float volume,
                                 bool  looping,
                                 float delay,
                                 float pitch,
                                 float range)
{
    AnonymousAudioPlayer::PlayAudioClip(audioClipFilepath, position, volume,
                                        looping, delay, pitch, range);
}

void AudioManager::PlaySound(const String &soundFilepath,
                             const Vector3& position,
                             float volume,
                             bool  looping,
                             float delay,
                             float pitch,
                             float range)
{
    AnonymousAudioPlayer::PlaySound(soundFilepath, position, volume, looping,
                                    delay, pitch, range);
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
