#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Component.h"

FORWARD class AudioClip;

class AudioSource : public Component
{
    COMPONENT(AudioSource)

public:
    enum State
    {
        Playing = AL_PLAYING,
        Paused  = AL_PAUSED,
        Stopped = AL_STOPPED
    };

    AudioSource();
    virtual ~AudioSource();

    void Play(float delay = 0.0f);
    void Pause();
    void Stop();

    void SetAudioClip(AudioClip *audioClip);
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetRange(float range);
    void SetLooping(bool looping);
    void SetPlayOnStart(bool playOnStart);

    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsStopped() const;
    State GetState() const;
    float GetPlayProgress() const;
    float GetVolume() const;
    float GetPitch()  const;
    float GetRange()  const;
    bool IsLooping()  const;
    bool IsPlayOnStart() const;
    ALuint GetALSourceId() const;
    AudioClip* GetAudioClip() const;

    // Component
    void OnStart() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    // Component
    virtual void OnUpdate() override;

private:
    ALuint m_alSourceId = 0;
    AudioClip *m_audioClip = nullptr;
    float m_test = 0.5f;
    float m_pitch = 1.0f;
    float m_range = 5.0f;
    float m_volume = 1.0f;
    bool  m_looping = false;
    bool  m_playOnStart = true;

    void UpdateALProperties() const;
    void SetAudioClipNoDettachAttach(AudioClip *audioClip);

    friend class AudioClip;
    friend class AudioManager;
};

#endif // AUDIOSOURCE_H
