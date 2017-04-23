#include "Bang/AudioClipAssetFile.h"

#include "Bang/IO.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/XMLParser.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"

#ifdef BANG_EDITOR
#include "Bang/SerializableObject.h"
#include "Bang/AudioClipAssetFileInspectable.h"
#endif

AudioClipAssetFile::AudioClipAssetFile()
{
}

AudioClipAssetFile::AudioClipAssetFile(
        const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    if (xmlInfo)
    {
        m_audioFilepath = xmlInfo->GetFilepath("AudioFilepath");
        delete xmlInfo;
    }
}

const QPixmap& AudioClipAssetFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/AudioClipIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Asset);
}

#ifdef BANG_EDITOR
SerializableObject *AudioClipAssetFile::GetNewInspectable() const
{
    return new AudioClipAssetFileInspectable(*this);
}
#endif

void AudioClipAssetFile::SetAudioFilepath(const String &audioFilepath)
{
    m_audioFilepath = audioFilepath;

    if (!m_audioFilepath.Empty())
    {
        AudioClip *audioClip = AssetsManager::Load<AudioClip>(GetAbsolutePath(),
                                                              false);
        if (audioClip)
        {
            if (!audioClip->LoadFromFile(m_audioFilepath))
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

bool AudioClipAssetFile::IsAsset() const
{
    return true;
}

