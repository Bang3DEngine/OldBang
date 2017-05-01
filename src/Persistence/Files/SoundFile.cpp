#include "Bang/SoundFile.h"

#include "Bang/IO.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"
#include "Bang/IconManager.h"
#include "Bang/FileInspectable.h"

SoundFile::SoundFile()
{
}

SoundFile::SoundFile(const Path& path)
    : File(path)
{

}

SoundFile::~SoundFile()
{
    if (m_tmpAudioSource) { delete m_tmpAudioSource; }
    if (m_tmpAudioClip)   { delete m_tmpAudioClip;   }
}

const QPixmap& SoundFile::GetIcon() const
{
    return IconManager::LoadPixmap(EPATH("Icons/AudioIcon.png"),
                                   IconManager::IconOverlay::Data);
}

void SoundFile::Read(const XMLNode &xmlInfo)
{

}

void SoundFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);

    xmlInfo->SetString("FileName", GetPath().GetNameExt(),
                       {XMLProperty::Readonly});

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

#ifdef BANG_EDITOR
IInspectable *SoundFile::GetNewInspectable()
{
    return new FileInspectable<SoundFile>(*this);
}

void SoundFile::OnButtonClicked(const String &attrName)
{
    ENSURE( IO::ExistsFile( GetPath().GetAbsolute() ) );
    if (attrName == "Play")
    {
        m_tmpAudioSource = new AudioSource();
        m_tmpAudioClip = new AudioClip();
        m_tmpAudioClip->LoadFromFile( GetPath() );
        m_tmpAudioSource->SetAudioClip(m_tmpAudioClip);
        m_tmpAudioSource->Play();
    }
    else if (attrName == "Stop")
    {
        delete m_tmpAudioSource; m_tmpAudioSource = nullptr;
        delete m_tmpAudioClip;   m_tmpAudioClip   = nullptr;
    }
}
#endif
