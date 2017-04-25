#include "Bang/SoundFile.h"

#include "Bang/IO.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"
#include "Bang/IconManager.h"
#include "Bang/FileInspectable.h"

#ifdef BANG_EDITOR
#include "Bang/SoundFileInspectable.h"
#endif

SoundFile::SoundFile(const QFileSystemModel *model, const QModelIndex &index)
    : File(model, index)
{

}

SoundFile::~SoundFile()
{
    if (m_tmpAudioSource) { delete m_tmpAudioSource; }
    if (m_tmpAudioClip)   { delete m_tmpAudioClip;   }
}

const QPixmap& SoundFile::GetIcon() const
{
    String path = IO::ToAbsolute("./Icons/AudioIcon.png", true);
    return IconManager::LoadPixmap(path, IconManager::IconOverlay::Data);
}

void SoundFile::Read(const XMLNode &xmlInfo)
{

}

void SoundFile::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("SoundFileInspectable");

    xmlInfo->SetString("FileName", GetNameAndExtension(), {XMLProperty::Readonly});

    SoundFile *noConstThis = const_cast<SoundFile*>(this);
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

void SoundFile::OnButtonClicked(const String &attrName)
{
    ENSURE( IO::ExistsFile( GetAbsolutePath() ) );
    if (attrName == "Play")
    {
        m_tmpAudioSource = new AudioSource();
        m_tmpAudioClip = new AudioClip();
        m_tmpAudioClip->LoadFromFile( GetAbsolutePath() );
        m_tmpAudioSource->SetAudioClip(m_tmpAudioClip);
        m_tmpAudioSource->Play();
    }
    else if (attrName == "Stop")
    {
        delete m_tmpAudioSource; m_tmpAudioSource = nullptr;
        delete m_tmpAudioClip;   m_tmpAudioClip   = nullptr;
    }
}

#ifdef BANG_EDITOR
IInspectable *SoundFile::GetNewInspectable() const
{
    return new FileInspectable<SoundFile>(*this);
}
#endif
