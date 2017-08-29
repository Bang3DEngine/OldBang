#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include "Bang/Component.h"

class AudioListener : public Component
{
    COMPONENT(AudioListener)

public:
    AudioListener();
    virtual ~AudioListener();

    // Component
    virtual void OnUpdate() override;

    // SerializableObject
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:

   void UpdateALProperties() const;
};

#endif // AUDIOLISTENER_H
