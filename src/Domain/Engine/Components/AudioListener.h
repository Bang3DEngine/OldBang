#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include "Bang/Component.h"

class AudioListener : public Component
{
    OBJECT(AudioListener)
    ICLONEABLE(AudioListener)
    COMPONENT_ICON(AudioListener, "Icons/AudioIcon.png")

public:
    AudioListener();
    virtual ~AudioListener();

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual void OnUpdate() override;

private:

   void UpdateALProperties() const;
};

#endif // AUDIOLISTENER_H
