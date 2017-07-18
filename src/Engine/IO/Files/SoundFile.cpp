#include "Bang/SoundFile.h"

#include "Bang/Paths.h"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"

SoundFile::SoundFile()
{
}

SoundFile::SoundFile(const Path& path)
    : BFile(path)
{

}

SoundFile::~SoundFile()
{
    if (m_tmpAudioSource) { delete m_tmpAudioSource; }
    if (m_tmpAudioClip)   { delete m_tmpAudioClip;   }
}

void SoundFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
}

void SoundFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);

    xmlInfo->SetString("FileName", GetPath().GetNameExt());
}
