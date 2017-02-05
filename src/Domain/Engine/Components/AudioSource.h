#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Component.h"
#include "AudioPlayProperties.h"
#include "IAttrWidgetButtonListener.h"

class XMLNode;
class AudioClip;
class ICloneable;
class AudioSource : public Component
                    #ifdef BANG_EDITOR
                    ,public IAttrWidgetButtonListener
                    #endif
{
public:
    enum State
    {
        Playing = AL_PLAYING,
        Paused  = AL_PAUSED,
        Stopped = AL_STOPPED
    };

    AudioSource();
    virtual ~AudioSource();

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    void Play(float delaySeconds = 0.0f);
    void Pause();
    void Stop();

    AudioClip* GetAudioClip() const;
    void SetAudioClip(AudioClip *audioClip);

    AudioPlayProperties GetAudioPlayProperties() const;

    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetLooping(bool looping);

    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsStopped() const;

    State GetState() const;

    float GetVolume() const;
    float GetPitch()  const;
    bool IsLooping()  const;

    virtual void OnButtonClicked(const String &attrName) override;

private:
    ALuint m_alSourceId = 0;

    AudioClip *m_audioClip = nullptr;
    float m_volume   = 1.0f;
    float m_pitch    = 1.0f;
    bool m_looping   = false;

    ALuint GetALSourceId() const;
    void SetAudioClipNoDettachAttach(AudioClip *audioClip);

    friend class AudioClip;
    friend class GameObject;
    friend class AudioClipAssetFileInspectable;
};

#endif // AUDIOSOURCE_H
