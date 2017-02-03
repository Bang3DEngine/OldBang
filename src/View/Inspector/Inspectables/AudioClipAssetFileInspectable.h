#ifndef AUDIOCLIPFILEINSPECTABLE_H
#define AUDIOCLIPFILEINSPECTABLE_H

#include "String.h"
#include "IInspectable.h"
#include "AudioClipAssetFile.h"
#include "IAttrWidgetButtonListener.h"

class XMLNode;
class AudioClipAssetFileInspectable : public IInspectable,
                                      public IAttrWidgetButtonListener
{
public:
    AudioClipAssetFileInspectable(const AudioClipAssetFile &audioClipAssetFile);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;

    virtual void OnButtonClicked(const String &attrName);

private:
    AudioClipAssetFile m_audioClipAssetFile;
};

#endif // AUDIOCLIPFILEINSPECTABLE_H
