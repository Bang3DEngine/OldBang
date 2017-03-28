#include "Bang/AudioFileInspectable.h"

#include "Bang/XMLNode.h"

AudioFileInspectable::AudioFileInspectable(const AudioFile &audioFile)
    : m_audioFile(audioFile)
{

}

void AudioFileInspectable::Read(const XMLNode &xmlInfo)
{
    // They all are readonly properties, so we wont do anything here
}

void AudioFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("AudioFileInspectable");

    xmlInfo->SetString("FileName", m_audioFile.GetNameAndExtension(), {XMLProperty::Readonly});
}
