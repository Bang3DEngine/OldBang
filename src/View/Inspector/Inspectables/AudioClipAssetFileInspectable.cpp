#include "AudioClipAssetFileInspectable.h"

#include "Debug.h"
#include "XMLNode.h"
#include "AudioClip.h"
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
    String audioFilepath = xmlInfo->GetFilepath("AudioFilepath");
    m_audioClipAssetFile.SetAudioFilepath(audioFilepath);

    String audioClipFilepath = m_audioClipAssetFile.GetAbsolutePath();
    if (m_tmpAudioSource)
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>(audioClipFilepath,
                                                              false);
        m_tmpAudioSource->SetAudioClip(audioClip);
    }

    AssetsManager::OnAssetFileChanged<AudioClip>(audioClipFilepath, xmlInfo);
}

void AudioClipAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    AudioClip *audioClip = GetRelatedAudioClip();

    xmlInfo->SetTagName("AudioClipAssetFileInspectable");

    xmlInfo->SetFilepath("AudioFilepath",
                         m_audioClipAssetFile.GetAudioFilepath(),
                         "ogg wav", {});

    xmlInfo->SetString("Length", String(audioClip->GetLength()) + " seconds",
                        {XMLProperty::Readonly});

    AudioClipAssetFileInspectable *noConstThis =
            const_cast<AudioClipAssetFileInspectable*>(this);
    bool isPlaying = m_tmpAudioSource && m_tmpAudioSource->IsPlaying();
    if (isPlaying)
    {
        xmlInfo->SetButton("Stop", noConstThis, {});
        xmlInfo->SetButton("Play", noConstThis, {XMLProperty::Hidden});
    }
    else
    {

        xmlInfo->SetButton("Stop", noConstThis, {XMLProperty::Hidden});
        xmlInfo->SetButton("Play", noConstThis, {});
    }
}

void AudioClipAssetFileInspectable::OnButtonClicked(const String &attrName)
{
    String audioClipFilepath = m_audioClipAssetFile.GetAbsolutePath();

    bool hasToPlay = !m_tmpAudioSource || !m_tmpAudioSource->IsPlaying();
    if (hasToPlay)
    {
        if (!m_tmpAudioSource)
        {
            m_tmpAudioSource = new AudioSource();
            AudioClip *audioClip = GetRelatedAudioClip();
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

AudioClip *AudioClipAssetFileInspectable::GetRelatedAudioClip() const
{
    return AssetsManager::Load<AudioClip>( m_audioClipAssetFile.GetAbsolutePath() );
}

