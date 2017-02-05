#include "AudioClipAssetFileInspectable.h"

#include "Debug.h"
#include "XMLNode.h"
#include "AudioClip.h"
#include "FileWriter.h"
#include "AudioSource.h"
#include "AudioManager.h"
#include "AssetsManager.h"

AudioClipAssetFileInspectable::AudioClipAssetFileInspectable(
        const AudioClipAssetFile &audioClipAssetFile)
    : m_audioClipAssetFile(audioClipAssetFile)
{

}

void AudioClipAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    m_audioClipAssetFile.SetAudioFilepath(xmlInfo->GetFilepath("AudioFilepath"));

    AssetsManager::OnAssetFileChanged<AudioClip>(
                m_audioClipAssetFile.GetAbsolutePath(), xmlInfo);
    FileWriter::WriteToFile(m_audioClipAssetFile.GetAbsolutePath(),
                            xmlInfo->ToString()); //Save
}

void AudioClipAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("AudioClipAssetFileInspectable");

    xmlInfo->SetFilepath("AudioFilepath",
                         m_audioClipAssetFile.GetAudioFilepath(),
                         "ogg wav", {});

    AudioClipAssetFileInspectable *noConstThis =
            const_cast<AudioClipAssetFileInspectable*>(this);
    String buttonText = "";
    if (m_tmpAudioSource && m_tmpAudioSource->IsPlaying())
    {
        buttonText = "Stop";
    }
    else
    {
        buttonText = "Play";
    }
    xmlInfo->SetButton(buttonText, noConstThis, {});
}

void AudioClipAssetFileInspectable::OnButtonClicked(const String &attrName)
{
    String audioFilepath = m_audioClipAssetFile.GetAbsolutePath();

    bool hasToPlay = !m_tmpAudioSource || !m_tmpAudioSource->IsPlaying();
    if (hasToPlay)
    {
        if (!m_tmpAudioSource)
        {
            m_tmpAudioSource = new AudioSource();
            AudioClip *audioClip = AssetsManager::Load<AudioClip>(audioFilepath);
            m_tmpAudioSource->SetAudioClip(audioClip);
        }
        m_tmpAudioSource->Play();
    }
    else
    {
        m_tmpAudioSource->Stop();
        delete m_tmpAudioSource;
        m_tmpAudioSource = nullptr;
    }
}

