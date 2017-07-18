#ifndef AUDIOCLIPASSETFILE_H
#define AUDIOCLIPASSETFILE_H

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/String.h"
#include "Bang/XMLNode.h"

class AudioClip;
class AudioSource;
class AudioClipFile : public BFile
{
public:
    AudioClipFile();
    AudioClipFile(const Path& path);

    virtual ~AudioClipFile();

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetSoundFilepath(const Path &audioFilepath);
    const Path& GetAudioFilepath() const;


    virtual bool IsAsset() const override;

private:
    Path m_audioFilepath;
    AudioSource *m_tmpAudioSource = nullptr;

    AudioClip *GetRelatedAudioClip() const;
};

#endif // AUDIOCLIPASSETFILE_H
