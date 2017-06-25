#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThreadPool>
#include <QMutexLocker>
#include "Bang/WinUndef.h"

#include "Bang/Math.h"
#include "Bang/Path.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"

class AudioClip;
class GameObject;
class AudioSource;
class AudioPlayerRunnable;
class AnonymousAudioPlayer;
class AudioManager
{
public:
    static AudioManager *GetInstance();

    static void PlayAudioClip(AudioClip *audioClip,
                              int alSourceId,
                              float delay = 0.0f);


    static void PlayAnonymousAudioClip(
                              const Path& audioClipFilepath,
                              const Vector3& position = Vector3::Zero,
                              float volume            = 1.0f,
                              bool  looping           = false,
                              float delay             = 0.0f,
                              float pitch             = 1.0f,
                              float range             = Math::Infinity<float>()
                              );

    static void PlaySound(const Path &soundFilepath,
                          const Vector3& position = Vector3::Zero,
                          float volume            = 1.0f,
                          bool  looping           = false,
                          float delay             = 0.0f,
                          float pitch             = 1.0f,
                          float range             = Math::Infinity<float>());

    static void PauseAllSounds();
    static void ResumeAllSounds();
    static void StopAllSounds();

    static void ClearALErrors();
    static bool CheckALError();

private:
    AudioManager();
    virtual ~AudioManager();

    QThreadPool m_threadPool;
    QMutex m_mutex_currentAudios;
    List<AudioPlayerRunnable*> m_currentAudios;
    AnonymousAudioPlayer *m_anonymousAudioPlayer = nullptr;

    AnonymousAudioPlayer *GetAnonymousAudioPlayer() const;

    void OnAudioFinishedPlaying(AudioPlayerRunnable *audioPlayer);

    friend class Application;
    friend class AudioPlayerRunnable;
    friend class AnonymousAudioPlayer;
};

#endif // AUDIOMANAGER_H
