#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThreadPool>
#include "Bang/WinUndef.h"

#include "Bang/Math.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"

class AudioClip;
class GameObject;
class AudioSource;
class AnonymousAudioPlayer;
class AudioManager
{
public:
    static AudioManager *GetInstance();

    static void PlayAudioClip(AudioClip *audioClip,
                              int alSourceId,
                              float delay = 0.0f);


    static void PlayAudioClip(const String& audioClipFilepath,
                              const Vector3& position = Vector3::Zero,
                              float volume            = 1.0f,
                              bool  looping           = false,
                              float delay             = 0.0f,
                              float pitch             = 1.0f,
                              float range             = Math::Infinity<float>()
                              );

    static void PlaySound(const String &soundFilepath,
                          const Vector3& position = Vector3::Zero,
                          float volume            = 1.0f,
                          bool  looping           = false,
                          float delay             = 0.0f,
                          float pitch             = 1.0f,
                          float range             = Math::Infinity<float>());

    static void ClearALErrors();
    static bool CheckALError();

private:
    AudioManager();
    virtual ~AudioManager();

    QThreadPool m_threadPool;
    AnonymousAudioPlayer *m_anonymousAudioPlayer = nullptr;

    AnonymousAudioPlayer *GetAnonymousAudioPlayer() const;

    friend class Application;
    friend class AnonymousAudioPlayer;
};

#endif // AUDIOMANAGER_H
