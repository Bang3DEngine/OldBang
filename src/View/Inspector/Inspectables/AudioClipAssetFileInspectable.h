#ifndef AUDIOCLIPFILEINSPECTABLE_H
#define AUDIOCLIPFILEINSPECTABLE_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "String.h"
#include "SerializableObject.h"
#include "AudioClipAssetFile.h"
#include "IAttrWidgetButtonListener.h"

class XMLNode;
class AudioClip;
class AudioSource;
class AudioClipAssetFileInspectable : public SerializableObject,
                                      public IAttrWidgetButtonListener
{
public:
    AudioClipAssetFileInspectable(const AudioClipAssetFile &audioClipAssetFile);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    virtual const QPixmap& GetIcon() const override;

    virtual void OnButtonClicked(const String &attrName);

private:
    AudioClipAssetFile m_audioClipAssetFile;
    AudioSource *m_tmpAudioSource = nullptr;

    AudioClip *GetRelatedAudioClip() const;
};

#endif // AUDIOCLIPFILEINSPECTABLE_H
