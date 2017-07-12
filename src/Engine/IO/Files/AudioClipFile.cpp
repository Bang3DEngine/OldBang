#include "Bang/AudioClipFile.h"

#include "Bang/Paths.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"
#include "Bang/AssetsManager.h"

AudioClipFile::AudioClipFile()
{
}

AudioClipFile::AudioClipFile(const Path& path)
    : BFile(path)
{
    XMLNode xmlInfo = XMLParser::FromFile(GetPath());
    m_audioFilepath = xmlInfo.GetFilepath("AudioFilepath");
}

AudioClipFile::~AudioClipFile()
{
    if (m_tmpAudioSource) { delete m_tmpAudioSource; }
}

const QPixmap& AudioClipFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/AudioClipIcon.png"),
                                   IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
void AudioClipFile::OnButtonClicked(const AttrWidgetButton *clickedButton)
{
    /*
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
    */
}
#endif

void AudioClipFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);

    Path soundFilepath = xmlInfo.GetFilepath("AudioFilepath");
    SetSoundFilepath(soundFilepath);

    if (m_tmpAudioSource)
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>( GetPath() );
        m_tmpAudioSource->SetAudioClip(audioClip);
    }

    #ifdef BANG_EDITOR
    AssetsManager::UpdateAsset(GetPath(), xmlInfo);
    #endif
}

void AudioClipFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    AudioClip *audioClip = GetRelatedAudioClip();

    xmlInfo->SetFilepath("AudioFilepath", GetAudioFilepath(), "ogg wav", {});

    xmlInfo->SetString("Length", String(audioClip->GetLength()) + " seconds",
                       {XMLProperty::Readonly});

    #ifdef BANG_EDITOR
    AudioClipFile *noConstThis = const_cast<AudioClipFile*>(this);
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
    #endif
}

void AudioClipFile::SetSoundFilepath(const Path &audioFilepath)
{
    m_audioFilepath = audioFilepath;

    if (!m_audioFilepath.IsEmpty())
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>(GetPath());
        if (audioClip)
        {
            if (!audioClip->LoadFromFile( GetPath() ))
            {
                // If loading fails, set to audioFilepath to ""
                SetSoundFilepath( Path() );
            }
        }
    }
}

const Path &AudioClipFile::GetAudioFilepath() const
{
    return m_audioFilepath;
}

bool AudioClipFile::IsAsset() const
{
    return true;
}

AudioClip *AudioClipFile::GetRelatedAudioClip() const
{
    return AssetsManager::Load<AudioClip>( GetPath() );
}

