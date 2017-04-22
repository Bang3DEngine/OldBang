#ifndef SOUNDFILEINSPECTABLE_H
#define SOUNDFILEINSPECTABLE_H

#include "Bang/SoundFile.h"
#include "Bang/SerializableObject.h"
#include "Bang/IAttrWidgetButtonListener.h"

class XMLNode;
class AudioClip;
class AudioSource;
class SoundFileInspectable : public SerializableObject,
                             public IAttrWidgetButtonListener
{
public:
    SoundFileInspectable(const SoundFile &audioFile);
    virtual ~SoundFileInspectable();

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual void OnButtonClicked(const String &attrName) override;

private:
    AudioSource* m_tmpAudioSource = nullptr;
    AudioClip* m_tmpAudioClip     = nullptr;
    SoundFile m_soundFile;
};

#endif // AUDIOFILEINSPECTABLE_H
