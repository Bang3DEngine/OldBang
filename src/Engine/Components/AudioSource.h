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

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void Play(float delay = 0.0f);
    void Pause();
    void Stop();

    AudioClip* GetAudioClip() const;
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

    void OnStart() override;

    /**
     * @brief Returns the progress of the AudioSource playing in the range [0.0, 1.0].
     */
    float GetPlayProgress() const;
    float GetVolume() const;
    float GetPitch()  const;
    float GetRange()  const;
    bool IsLooping()  const;
    bool IsPlayOnStart() const;

    ALuint GetALSourceId() const;

protected:
    virtual void OnUpdate() override;

private:
    ALuint m_alSourceId = 0;

    AudioClip *m_audioClip = nullptr;

    float m_test = 0.5f;

    float m_volume = 1.0f;
    float m_pitch = 1.0f;

    bool  m_looping        = false;
    float m_range          = 5.0f;
    bool  m_playOnStart    = true;

    void UpdateALProperties() const;
    void SetAudioClipNoDettachAttach(AudioClip *audioClip);

    friend class AudioClip;
    friend class AudioManager;
};

#endif // AUDIOSOURCE_H
