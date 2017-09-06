#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class AudioListener : public Component
{
    COMPONENT(AudioListener)

public:
    AudioListener();
    virtual ~AudioListener();

    // Component
    virtual void OnUpdate() override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:

   void UpdateALProperties() const;
};

NAMESPACE_BANG_END

#endif // AUDIOLISTENER_H
