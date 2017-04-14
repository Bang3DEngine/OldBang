#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Component.h"

#ifdef BANG_EDITOR
#include "Bang/IAttrWidgetButtonListener.h"
#endif

class XMLNode;
class AudioClip;
class ICloneable;
class AudioSource : public Component
                    #ifdef BANG_EDITOR
                    ,public IAttrWidgetButtonListener
                    #endif
{
    OBJECT(AudioSource)
    COMPONENT_ICON(AudioSource, "Icons/AudioIcon.png")

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

    #ifdef BANG_EDITOR
    virtual void OnButtonClicked(const String &attrName) override;
    #endif

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void Play(float delaySeconds = 0.0f);
    void Pause();
    void Stop();

    AudioClip* GetAudioClip() const;
    void SetAudioClip(AudioClip *audioClip);

    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetRange(float range);
    void SetLooping(bool looping);

    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsStopped() const;

    State GetState() const;

    /**
     * @brief Returns the progress of the AudioSource playing in the range [0.0, 1.0].
     */
    float GetPlayProgress() const;
    float GetVolume() const;
    float GetPitch()  const;
    float GetRange()  const;
    bool IsLooping()  const;

protected:
    virtual void OnUpdate() override;
    virtual void OnDrawGizmos(bool depthed, bool overlay) override;

private:
    ALuint m_alSourceId = 0;

    AudioClip *m_audioClip = nullptr;
    float m_volume   = 1.0f;
    float m_pitch    = 1.0f;
    bool m_looping   = false;
    float m_range    = 5.0f;

    ALuint GetALSourceId() const;
    void UpdateALProperties() const;
    void SetAudioClipNoDettachAttach(AudioClip *audioClip);

    friend class AudioClip;
    friend class AudioClipAssetFileInspectable;
};

#endif // AUDIOSOURCE_H
