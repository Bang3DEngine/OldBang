#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Bang/Component.h"
#include "Bang/AudioManager.h"
#include "Bang/ALAudioSource.h"

NAMESPACE_BANG_BEGIN

FORWARD class AudioClip;

class AudioSource : public Component,
                    public ALAudioSource
{
    COMPONENT(AudioSource)

public:
    virtual ~AudioSource();

    void SetAudioClip(AudioClip *audioClip);
    void SetPlayOnStart(bool playOnStart);

    void Play(); // Hides ALAudioSource::Play()
    void Play(float delay);

    bool IsPlayOnStart() const;
    float GetPlayProgress() const;
    AudioClip* GetAudioClip() const;

    // Component
    void OnStart() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    AudioSource();

    // Component
    virtual void OnUpdate() override;

private:
    AudioClip *m_audioClip = nullptr;
    bool m_playOnStart = true;

    uint m_currentAudioClipALBufferId = 0;

    friend class AudioClip;
    friend class AudioManager;
};

NAMESPACE_BANG_END

#endif // AUDIOSOURCE_H
