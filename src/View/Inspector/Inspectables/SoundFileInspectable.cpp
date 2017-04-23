#include "Bang/SoundFileInspectable.h"

#include "Bang/IO.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"
#include "Bang/AudioManager.h"

SoundFileInspectable::SoundFileInspectable(const SoundFile &soundFile)
    : m_soundFile(soundFile)
{
}

SoundFileInspectable::~SoundFileInspectable()
{
    if (m_tmpAudioSource) { delete m_tmpAudioSource; }
    if (m_tmpAudioClip)   { delete m_tmpAudioClip;   }
}

void SoundFileInspectable::Read(const XMLNode &xmlInfo)
{
    // They all are readonly properties, so we wont do anything here
}

void SoundFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("SoundFileInspectable");

    xmlInfo->SetString("FileName", m_soundFile.GetNameAndExtension(),
                       {XMLProperty::Readonly});

    SoundFileInspectable *noConstThis = const_cast<SoundFileInspectable*>(this);
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

void SoundFileInspectable::OnButtonClicked(const String &attrName)
{
    ENSURE(IO::ExistsFile(m_soundFile.GetAbsolutePath()));
    if (attrName == "Play")
    {
        m_tmpAudioSource = new AudioSource();
        m_tmpAudioClip = new AudioClip();
        m_tmpAudioClip->LoadFromFile(m_soundFile.GetAbsolutePath());
        m_tmpAudioSource->SetAudioClip(m_tmpAudioClip);
        m_tmpAudioSource->Play();
    }
    else if (attrName == "Stop")
    {
        delete m_tmpAudioSource; m_tmpAudioSource = nullptr;
        delete m_tmpAudioClip;   m_tmpAudioClip   = nullptr;
    }
}
