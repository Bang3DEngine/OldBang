#include "Bang/SoundFileInspectable.h"

#include "Bang/IO.h"
#include "Bang/XMLNode.h"
#include "Bang/AudioManager.h"

SoundFileInspectable::SoundFileInspectable(const SoundFile &soundFile)
    : m_soundFile(soundFile)
{

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

    xmlInfo->SetButton("Play", const_cast<SoundFileInspectable*>(this));
}

void SoundFileInspectable::OnButtonClicked(const String &attrName)
{
    ENSURE(IO::ExistsFile(m_soundFile.GetAbsolutePath()));
    if (attrName == "Play")
    {
        AudioManager::PlaySound(m_soundFile.GetAbsolutePath());
    }
}
