#ifndef AUDIOFILEINSPECTABLE_H
#define AUDIOFILEINSPECTABLE_H

#include "AudioFile.h"
#include "IInspectable.h"

class XMLNode;
class AudioFileInspectable : public IInspectable
{
public:
    AudioFileInspectable(const AudioFile &audioFile);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;

private:
    AudioFile m_audioFile;
};

#endif // AUDIOFILEINSPECTABLE_H
