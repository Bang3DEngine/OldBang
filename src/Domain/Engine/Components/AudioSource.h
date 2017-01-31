#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Component.h"

class XMLNode;
class AudioClip;
class ICloneable;
class AudioSource : public Component
{
public:
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

    void Play();
    void Pause();
    void Stop();

    AudioClip* GetAudioClip() const;
    void SetAudioClip(AudioClip *audioClip);

private:
    AudioClip *m_audioClip = nullptr;

    friend class GameObject;
};

#endif // AUDIOSOURCE_H
