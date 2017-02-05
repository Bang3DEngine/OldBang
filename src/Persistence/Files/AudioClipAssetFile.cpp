#include "AudioClipAssetFile.h"

#include "XMLNode.h"
#include "AudioClip.h"
#include "XMLParser.h"
#include "Persistence.h"
#include "AssetsManager.h"
#include "AudioClipAssetFileInspectable.h"

#ifdef BANG_EDITOR
#include "IInspectable.h";
#endif

AudioClipAssetFile::AudioClipAssetFile(
        const QFileSystemModel *model, const QModelIndex &index) : File(model, index)
{
    XMLNode *xmlInfo;
    xmlInfo = XMLParser::FromFile(m_path);
    if (xmlInfo)
    {
        m_audioFilepath = xmlInfo->GetFilepath("AudioFilepath");
        delete xmlInfo;
    }
}

QPixmap AudioClipAssetFile::GetIcon() const
{
    String fp = Persistence::ToAbsolute("./Icons/AudioClipIcon.png", true);
    QPixmap pm(fp.ToQString());
    return pm;
}

IInspectable *AudioClipAssetFile::GetInspectable() const
{
    return new AudioClipAssetFileInspectable(*this);
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

