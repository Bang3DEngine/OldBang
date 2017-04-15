#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThreadPool>
#include "Bang/WinUndef.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Math.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"

class AudioClip;
class GameObject;
class AudioSource;
class AudioManager
{
public:
    static AudioManager *GetInstance();

    static void PlayAudioClip(const String& audioClipFilepath,
                              const Vector3& position = Vector3::Zero,
                              float volume            = 1.0f,
                              float delay             = 0.0f,
                              float pitch             = 1.0f,
                              float range             = Math::Infinity<float>()
                              );

    static void PlayAudioClip(AudioClip *audioClip,
                              int alSourceId,
                              float delay = 0.0f);

    static void PlaySound(const String &soundFilepath,
                          const Vector3& position = Vector3::Zero,
                          float volume            = 1.0f,
                          float delay             = 0.0f,
                          float pitch             = 1.0f,
                          float range             = Math::Infinity<float>());

    static void ClearALErrors();
    static bool CheckALError();

private:
    AudioManager();
    virtual ~AudioManager();

    QThreadPool m_threadPool;
    List< std::pair<AudioSource*, AudioClip*> >
                                m_anonymousAudioSources_audioClips;
    GameObject *m_currentListener = nullptr;

    static AudioSource *CreateAudioSource(
                    const Vector3& position = Vector3::Zero,
                    float volume            = 1.0f,
                    float pitch             = 1.0f,
                    float range             = Math::Infinity<float>());

    friend class Application;
};

#endif // AUDIOMANAGER_H
