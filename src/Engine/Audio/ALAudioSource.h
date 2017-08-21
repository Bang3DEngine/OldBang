#ifndef ALAUDIOSOURCE_H
#define ALAUDIOSOURCE_H

#include <AL/al.h>
#include <AL/alc.h>

#include "Bang/AudioParams.h"

class ALAudioSource
{
public:
    enum State
    {
        Playing = AL_PLAYING,
        Paused  = AL_PAUSED,
        Stopped = AL_STOPPED
    };

    ALAudioSource();
    virtual ~ALAudioSource();

    void Play();
    void Pause();
    void Stop();

    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetRange(float range);
    void SetLooping(bool looping);
    void SetPosition(const Vector3& position);
    void SetParams(const AudioParams &audioParams);
    void SetALBufferId(ALuint bufferId);

    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsStopped() const;
    State GetState() const;
    float GetVolume() const;
    float GetPitch()  const;
    float GetRange()  const;
    ALuint GetALSourceId() const;
    const Vector3& GetPosition() const;
    const AudioParams& GetParams();
    bool IsLooping()  const;

private:
    ALuint m_alSourceId = 0;
    AudioParams m_audioParams;
    bool m_autoDelete = false;

    friend class AudioManager;
    friend class AudioPlayerRunnable;
};

#endif // ALAUDIOSOURCE_H
