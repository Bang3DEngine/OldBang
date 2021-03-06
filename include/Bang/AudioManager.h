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
#include "Bang/ResourceHandle.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Path;
FORWARD class AudioClip;
FORWARD class GameObject;
FORWARD class AudioSource;
FORWARD class ALAudioSource;
FORWARD class AudioPlayerRunnable;

class AudioManager : public IDestroyListener
{
public:
    void Init();

    static void Play(AudioClip* audioClip,
                     ALAudioSource *alAudioSource,
                     float delay = 0.0f);
    static void Play(AudioClip* audioClip,
                     const AudioParams &params,
                     float delay = 0.0f);
    static void Play(const Path& audioClipFilepath,
                     const AudioParams &params,
                     float delay = 0.0f);

    static void PauseAllSounds();
    static void ResumeAllSounds();
    static void StopAllSounds();
    static void SetPlayOnStartBlocked(bool blocked);

    static bool GetPlayOnStartBlocked();
    static void ClearALErrors();
    static bool CheckALError();

    static AudioManager *GetInstance();

private:
    ALCdevice *m_alDevice = nullptr;
    ALCcontext *m_alContext = nullptr;

    ThreadPool m_threadPool;
    Mutex m_mutexCurrentAudios;
    bool m_playOnStartBlocked = false;
    Map<ALAudioSource*, AudioPlayerRunnable*> m_sourcesToPlayers;

    AudioManager();
    virtual ~AudioManager();

    bool InitAL();
    static List<String> GetAudioDevicesList();
    static String GetALErrorEnumString(ALenum errorEnum);
    static String GetALCErrorEnumString(ALCenum errorEnum);

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;
    void OnAudioPlayerDestroyed(AudioPlayerRunnable *audioPlayer);
    void OnALAudioSourceDestroyed(ALAudioSource *alAudioSource);

    // Handling of real-time buffer change
    static void DettachSourcesFromAudioClip(AudioClip *ac);

    void OnAudioFinishedPlaying(AudioPlayerRunnable *audioPlayer);

    friend class AudioClip;
    friend class Application;
    friend class AudioPlayerRunnable;
};

NAMESPACE_BANG_END

#endif // AUDIOMANAGER_H
