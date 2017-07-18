#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include <QPixmap>

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"

class AudioClip;
class AudioSource;
class SoundFile : public BFile
{
public:
    SoundFile();
    SoundFile(const Path& path);
    virtual ~SoundFile();

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    AudioSource* m_tmpAudioSource = nullptr;
    AudioClip* m_tmpAudioClip     = nullptr;

};

#endif // AUDIOFILE_H
