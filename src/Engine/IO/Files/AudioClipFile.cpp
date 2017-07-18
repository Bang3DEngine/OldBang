#include "Bang/AudioClipFile.h"

#include "Bang/Paths.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/XMLParser.h"
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
}

void AudioClipFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    AudioClip *audioClip = GetRelatedAudioClip();

    xmlInfo->SetFilepath("AudioFilepath", GetAudioFilepath());

    xmlInfo->SetString("Length", String(audioClip->GetLength()) + " seconds");
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

