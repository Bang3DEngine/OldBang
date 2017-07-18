#ifndef ANONYMOUSAUDIOPLAYER_H
#define ANONYMOUSAUDIOPLAYER_H

#include <QThreadPool>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Path.h"
#include "Bang/Math.h"
#include "Bang/List.h"
#include "Bang/Time.h"
#include "Bang/Vector3.h"

class AudioClip;
class GameObject;
class AudioSource;
class AnonymousAudioPlayer
{
public:
    static AnonymousAudioPlayer *GetInstance();

    static void PlayAudioClip(const Path& audioClipFilepath,
                              const Vector3& position,
                              float volume,
                              bool  looping,
                              float delay,
                              float pitch,
                              float range);

    static void PlaySound(const Path &soundFilepath,
                          const Vector3& position,
                          float volume,
                          bool  looping,
                          float delay,
                          float pitch,
                          float range);

    static void ClearALErrors();
    static bool CheckALError();

private:
    AnonymousAudioPlayer();

    struct PlayRequest
    {
        AudioSource *audioSource = nullptr;
        AudioClip *audioClip = nullptr;
        EpochTime timeToBeDestroyed = 0;
        bool deleteAudioClip = true;
    };
    List<PlayRequest> m_playRequests;

    static AudioSource *CreateAudioSource(
                    const Vector3& position,
                    float volume,
                    bool  looping,
                    float pitch,
                    float range);

    static void ClearAnonymousAudioSourcesAndClips();
    static void StopAndClearEverything();

    static void CreateAndStartPlayRequest(AudioSource *audioSource,
                                          AudioClip *audioClip,
                                          float delay,
                                          bool deleteAudioClip);

    friend class Application;
    friend class AudioManager;
};

#endif // ANONYMOUSAUDIOPLAYER_H
