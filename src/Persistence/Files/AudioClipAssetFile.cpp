#include "Bang/AudioClipAssetFile.h"

#include "Bang/IO.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

AudioClipAssetFile::AudioClipAssetFile()
{
}

AudioClipAssetFile::AudioClipAssetFile(
        const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    XMLNode xmlInfo = XMLParser::FromFile(m_path);
    m_audioFilepath = xmlInfo.GetFilepath("AudioFilepath");
}

AudioClipAssetFile::~AudioClipAssetFile()
{
    if (m_tmpAudioSource) { delete m_tmpAudioSource; }
}

const QPixmap& AudioClipAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/AudioClipIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
IInspectable *AudioClipAssetFile::GetNewInspectable()
{
    return new FileInspectable<AudioClipAssetFile>(*this);
}
#endif

void AudioClipAssetFile::Read(const XMLNode &xmlInfo)
{
    String audioFilepath = xmlInfo.GetFilepath("AudioFilepath");
    SetAudioFilepath(audioFilepath);

    String audioClipFilepath = GetAbsolutePath();
    if (m_tmpAudioSource)
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>(audioClipFilepath,
                                                              false);
        m_tmpAudioSource->SetAudioClip(audioClip);
    }

    AssetsManager::UpdateAsset(audioClipFilepath, xmlInfo);
}

void AudioClipAssetFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);

    AudioClip *audioClip = GetRelatedAudioClip();

    xmlInfo->SetFilepath("AudioFilepath", GetAudioFilepath(), "ogg wav", {});

    xmlInfo->SetString("Length", String(audioClip->GetLength()) + " seconds",
                       {XMLProperty::Readonly});

    AudioClipAssetFile *noConstThis = const_cast<AudioClipAssetFile*>(this);
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

void AudioClipAssetFile::SetAudioFilepath(const String &audioFilepath)
{
    m_audioFilepath = audioFilepath;

    if (!m_audioFilepath.Empty())
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>(GetAbsolutePath(),
                                                              false);
        if (audioClip)
        {
            if (!audioClip->LoadFromFile( GetAbsolutePath() ))
            {
                // If loading fails, set to audioFilepath to ""
                SetAudioFilepath("");
            }
        }
    }
}

const String &AudioClipAssetFile::GetAudioFilepath() const
{
    return m_audioFilepath;
}

void AudioClipAssetFile::OnButtonClicked(const String &attrName)
{
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

bool AudioClipAssetFile::IsAsset() const
{
    return true;
}

AudioClip *AudioClipAssetFile::GetRelatedAudioClip() const
{
    return AssetsManager::Load<AudioClip>( GetAbsolutePath() );
}

