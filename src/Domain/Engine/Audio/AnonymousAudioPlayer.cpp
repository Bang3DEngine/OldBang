#include "AnonymousAudioPlayer.h"

#include "Bang/IO.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"
#include "Bang/SceneManager.h"
#include "Bang/AudioManager.h"
#include "Bang/AssetsManager.h"

AnonymousAudioPlayer::AnonymousAudioPlayer()
{
}

AnonymousAudioPlayer *AnonymousAudioPlayer::GetInstance()
{
    return Application::GetInstance()->GetAudioManager()->
                        GetAnonymousAudioPlayer();
}

void AnonymousAudioPlayer::PlayAudioClip(const String &audioClipFilepath,
                                         const Vector3 &position,
                                         float volume,
                                         bool looping,
                                         float delay,
                                         float pitch,
                                         float range)
{
    AnonymousAudioPlayer::ClearAnonymousAudioSourcesAndClips();

    String absAudioClipFilepath = IO::ToAbsolute(audioClipFilepath, false);
    if (!IO::ExistsFile(absAudioClipFilepath))
    {
        Debug_Warn("Audio '" << audioClipFilepath << "' could not be found");
        return;
    }

    AudioClip *audioClip = AssetsManager::Load<AudioClip>(absAudioClipFilepath);
    AudioSource *audioSource =
            AnonymousAudioPlayer::CreateAudioSource(position, volume, looping,
                                                    pitch, range);
    AnonymousAudioPlayer::CreateAndStartPlayRequest(audioSource, audioClip,
                                                    delay, false);
}

void AnonymousAudioPlayer::PlaySound(const String &soundFilepathStr,
                                     const Vector3 &position,
                                     float volume,
                                     bool looping,
                                     float delay,
                                     float pitch,
                                     float range)
{
    AnonymousAudioPlayer::ClearAnonymousAudioSourcesAndClips();

    Path soundFilepath = Path(soundFilepathStr);
    if (!soundFilepath.Exists())
    {
        Debug_Warn("Sound '" << soundFilepathStr << "' could not be found");
        return;
    }

    AudioClip *audioClip = new AudioClip();
    audioClip->LoadFromFile(soundFilepath);
    AudioSource *audioSource =
            AnonymousAudioPlayer::CreateAudioSource(position, volume, looping,
                                                    pitch, range);
    AnonymousAudioPlayer::CreateAndStartPlayRequest(audioSource, audioClip,
                                                    delay, true);
}

AudioSource *AnonymousAudioPlayer::CreateAudioSource(const Vector3& position,
                                                     float volume,
                                                     bool looping,
                                                     float pitch,
                                                     float range)
{
    AudioSource *audioSource = new AudioSource();
    audioSource->SetVolume(volume);
    audioSource->SetPitch(pitch);
    audioSource->SetRange(range);
    audioSource->SetLooping(looping);
    alSourcefv(audioSource->GetALSourceId(), AL_POSITION, position.Values());
    return audioSource;
}

void AnonymousAudioPlayer::ClearAnonymousAudioSourcesAndClips()
{
    AnonymousAudioPlayer *aap = AnonymousAudioPlayer::GetInstance();

    for (auto it = aap->m_playRequests.Begin();
         it != aap->m_playRequests.End(); ++it)
    {
        const PlayRequest& playRequest = *it;
        if (Time::GetNow() >= playRequest.timeToBeDestroyed)
        {
            delete playRequest.audioSource;
            if (playRequest.deleteAudioClip) { delete playRequest.audioClip; }
            aap->m_playRequests.Remove(it++);
        }
    }
}

void AnonymousAudioPlayer::StopAndClearEverything()
{
    AnonymousAudioPlayer *aap = AnonymousAudioPlayer::GetInstance();
    for (const PlayRequest& playRequest : aap->m_playRequests)
    {
        delete playRequest.audioSource;
        if (playRequest.deleteAudioClip) { delete playRequest.audioClip; }
    }
    aap->m_playRequests.Clear();
}

void AnonymousAudioPlayer::CreateAndStartPlayRequest(AudioSource *audioSource,
                                                     AudioClip *audioClip,
                                                     float delay,
                                                     bool deleteAudioClip)
{
    PlayRequest playRequest;
    playRequest.audioClip       = audioClip;
    playRequest.audioSource     = audioSource;
    playRequest.deleteAudioClip = deleteAudioClip;

    bool looping   = playRequest.audioSource->IsLooping();
    float pitch    = playRequest.audioSource->GetPitch();
    float length   = playRequest.audioClip->GetLength();
    float duration = !looping ? (length / pitch) : Math::Infinity<float>();

    playRequest.timeToBeDestroyed = Time::GetNow() +
                                    EpochTime(duration * 1000) +
                                    EpochTime(delay * 1000);

    AnonymousAudioPlayer *aap = AnonymousAudioPlayer::GetInstance();
    aap->m_playRequests.PushBack(playRequest);

    playRequest.audioSource->SetAudioClip(playRequest.audioClip);
    playRequest.audioSource->Play(delay);
}
