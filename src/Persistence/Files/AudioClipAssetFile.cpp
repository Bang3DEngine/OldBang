#include "AudioClipAssetFile.h"

#include "XMLNode.h"
#include "XMLParser.h"
#include "Persistence.h"
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
    String fp = Persistence::ToAbsolute("./Icons/OtherFileIcon.png", true);
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
}

const String &AudioClipAssetFile::GetAudioFilepath() const
{
    return m_audioFilepath;
}

