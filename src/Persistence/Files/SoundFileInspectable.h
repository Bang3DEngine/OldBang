#ifndef SOUNDFILEINSPECTABLE_H
#define SOUNDFILEINSPECTABLE_H

#include "Bang/SoundFile.h"
#include "Bang/SerializableObject.h"
#include "Bang/IAttrWidgetButtonListener.h"

class XMLNode;
class SoundFileInspectable : public SerializableObject,
                             public IAttrWidgetButtonListener
{
public:
    SoundFileInspectable(const SoundFile &audioFile);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual void OnButtonClicked(const String &attrName) override;

private:
    SoundFile m_soundFile;
};

#endif // AUDIOFILEINSPECTABLE_H
