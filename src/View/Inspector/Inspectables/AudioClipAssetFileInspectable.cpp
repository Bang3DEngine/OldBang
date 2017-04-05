#include "Bang/AudioClipAssetFileInspectable.h"

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"
#include "Bang/AssetsManager.h"

AudioClipAssetFileInspectable::AudioClipAssetFileInspectable(
        const AudioClipAssetFile &audioClipAssetFile)
    : m_audioClipAssetFile(audioClipAssetFile)
{

}

void AudioClipAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    String audioFilepath = xmlInfo.GetFilepath("AudioFilepath");
    m_audioClipAssetFile.SetAudioFilepath(audioFilepath);

    String audioClipFilepath = m_audioClipAssetFile.GetAbsolutePath();
    if (m_tmpAudioSource)
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>(audioClipFilepath,
                                                              false);
        m_tmpAudioSource->SetAudioClip(audioClip);
    }

    AssetsManager::UpdateAsset(audioClipFilepath, xmlInfo);
}

void AudioClipAssetFileInspectable::Write(XMLNode *xmlInfo) const
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

const QPixmap &AudioClipAssetFileInspectable::GetIcon() const
{
    return AudioClip::GetIconStatic();
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

