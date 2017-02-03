#include "AudioFileInspectable.h"

#include "XMLNode.h"

AudioFileInspectable::AudioFileInspectable(const AudioFile &audioFile)
    : m_audioFile(audioFile)
{

}

void AudioFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    // They all are readonly properties, so we wont do anything here
}

void AudioFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("AudioFileInspectable");

    xmlInfo->SetString("FileName", m_audioFile.GetNameAndExtension(), {XMLProperty::Readonly});
}
