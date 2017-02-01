#include "AudioClipAssetFileInspectable.h"

#include "Debug.h"
#include "XMLNode.h"
#include "FileWriter.h"

AudioClipAssetFileInspectable::AudioClipAssetFileInspectable(
        const AudioClipAssetFile &audioClipAssetFile)
    : m_audioClipAssetFile(audioClipAssetFile)
{

}

void AudioClipAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    m_audioClipAssetFile.SetAudioFilepath(xmlInfo->GetFilepath("AudioFilepath"));
    Debug_Log(m_audioClipAssetFile.GetAbsolutePath());
    FileWriter::WriteToFile(m_audioClipAssetFile.GetAbsolutePath(),
                            xmlInfo->ToString()); //Save
}

void AudioClipAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("AudioClipAssetFileInspectable");
    xmlInfo->SetFilepath("AudioFilepath", m_audioClipAssetFile.GetAudioFilepath(),
                         "ogg wav", {});
}

