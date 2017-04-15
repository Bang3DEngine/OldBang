#include "Bang/AudioManager.h"

#include <QThreadPool>
#include "Bang/WinUndef.h"

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

AudioManager::AudioManager()
{
    alutInit(0, NULL);
    m_threadPool.setMaxThreadCount(256);
}

AudioManager::~AudioManager()
{
    alutExit();
}

void AudioManager::PlayAudioClip(const String &audioClipFilepath,
                                 const Vector3& position,
                                 float volume,
                                 float delay,
                                 float pitch,
                                 float range)
{
    String absAudioClipFilepath = IO::ToAbsolute(audioClipFilepath, false);
    if (!IO::ExistsFile(absAudioClipFilepath))
    {
        Debug_Warn("Audio file '" << audioClipFilepath <<
                   "' could not be found");
        return;
    }

    AudioClip *audioClip = AssetsManager::Load<AudioClip>(absAudioClipFilepath);
    AudioSource *audioSource = AudioManager::CreateAudioSource(position, volume,
                                                               pitch, range);
    audioSource->SetAudioClip(audioClip);
    AudioManager::PlayAudioClip(audioClip, audioSource->GetALSourceId(), delay);
}

void AudioManager::PlayAudioClip(AudioClip *audioClip,
                                 int alSourceId,
                                 float delay)
{
    ENSURE(audioClip);
    AudioManager *audioManager = AudioManager::GetInstance();

    AudioListener *listener = SceneManager::GetActiveScene()->
            GetComponentInChildren<AudioListener>();
    if (!listener)
    {
        Debug_Warn ("The scene does not contain an AudioListener");
    }

    AudioPlayerRunnable *player = new AudioPlayerRunnable(audioClip, alSourceId,
                                                          delay);
    audioManager->m_threadPool.tryStart(player);
}

void AudioManager::PlaySound(const String &soundFilepath,
                             const Vector3& position,
                             float volume,
                             float delay,
                             float pitch,
                             float range)
{
    String absSoundFilepath = IO::ToAbsolute(soundFilepath, false);
    if (!IO::ExistsFile(absSoundFilepath))
    {
        Debug_Warn("Audio file '" << soundFilepath << "' could not be found");
        return;
    }
    AudioManager *am = AudioManager::GetInstance();

    // Clean up old finished anonymous sound playings
    for (auto it = am->m_anonymousAudioSources_audioClips.Begin();
         it != am->m_anonymousAudioSources_audioClips.End(); ++it)
    {
        AudioSource *audioSource = it->first;
        AudioClip *audioClip     = it->second;
        if (!audioSource->IsPlaying())
        {
            audioSource->Stop();
            delete audioSource; delete audioClip;
            am->m_anonymousAudioSources_audioClips.Remove(it++);
        }
    }

    // Load the audioClip, create the anonymous audioSource, and play it
    AudioClip *audioClip = new AudioClip();
    audioClip->LoadFromFile(absSoundFilepath);

    AudioSource *audioSource = CreateAudioSource(position, volume,
                                                 pitch, range);
    am->m_anonymousAudioSources_audioClips.PushBack(
                std::make_pair(audioSource, audioClip) );
    audioSource->SetAudioClip(audioClip);
    audioSource->Play(delay);
}

AudioSource *AudioManager::CreateAudioSource(const Vector3& position,
                                             float volume,
                                             float pitch,
                                             float range)
{
    AudioSource *audioSource = new AudioSource();
    audioSource->SetVolume(volume);
    audioSource->SetPitch(pitch);
    audioSource->SetRange(range);
    alSourcefv(audioSource->GetALSourceId(), AL_POSITION, position.Values());
    return audioSource;
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
