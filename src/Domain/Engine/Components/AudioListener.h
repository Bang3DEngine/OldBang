#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include "Component.h"

class AudioListener : public Component
{
public:
    AudioListener();
    virtual ~AudioListener();

    virtual String GetName() const override;

    virtual ICloneable* CloneVirtual() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual void OnUpdate() override;

private:

   void UpdateALProperties() const;
};

#endif // AUDIOLISTENER_H
