#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Bang/Component.h"
#include "Bang/AudioManager.h"
#include "Bang/ALAudioSource.h"

FORWARD class AudioClip;

class AudioSource : public Component,
                    public ALAudioSource
{
    COMPONENT(AudioSource)

public:
    virtual ~AudioSource();

    void SetAudioClip(AudioClip *audioClip);
    void SetPlayOnStart(bool playOnStart);

    bool IsPlayOnStart() const;
    float GetPlayProgress() const;
    AudioClip* GetAudioClip() const;

    // Component
    void OnStart() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    AudioSource();

    // Component
    virtual void OnUpdate() override;

private:
    AudioClip *m_audioClip = nullptr;
    bool m_playOnStart = true;

    void SetAudioClipNoDettachAttach(AudioClip *audioClip);

    friend class AudioClip;
    friend class AudioManager;
};

#endif // AUDIOSOURCE_H
