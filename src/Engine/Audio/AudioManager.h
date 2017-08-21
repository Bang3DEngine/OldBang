#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>

#include "Bang/Set.h"
#include "Bang/Math.h"
#include "Bang/List.h"
#include "Bang/Vector3.h"
#include "Bang/ThreadPool.h"
#include "Bang/MutexLocker.h"
#include "Bang/AudioParams.h"

FORWARD class Path;
FORWARD class AudioClip;
FORWARD class GameObject;
FORWARD class AudioSource;
FORWARD class ALAudioSource;
FORWARD class AudioPlayerRunnable;

class AudioManager
{
public:
    static void Play(AudioClip *audioClip,
                     ALAudioSource *alAudioSource,
                     float delay = 0.0f);
    static void Play(AudioClip *audioClip,
                     const AudioParams &params,
                     float delay = 0.0f);
    static void Play(const Path& audioClipFilepath,
                     const AudioParams &params,
                     float delay = 0.0f);

    static void PauseAllSounds();
    static void ResumeAllSounds();
    static void StopAllSounds();

    static void ClearALErrors();
    static bool CheckALError();

    static AudioManager *GetInstance();

private:
    ALCdevice *m_alDevice = nullptr;
    ALCcontext *m_alContext = nullptr;

    AudioManager();
    virtual ~AudioManager();

    bool InitAL();

    ThreadPool m_threadPool;
    Mutex m_mutex_currentAudios;
    Set<AudioPlayerRunnable*> m_currentAudioPlayers;

    // Handling of real-time buffer change
    static void DettachSourcesFromAudioClip(AudioClip *ac);

    void OnAudioFinishedPlaying(AudioPlayerRunnable *audioPlayer);

    friend class AudioClip;
    friend class Application;
    friend class AudioPlayerRunnable;
};

#endif // AUDIOMANAGER_H
