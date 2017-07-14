#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/String.h"
#include "Bang/Paths.h"

class AudioSource;
class AudioClip : public Asset
{
    OBJECT(AudioClip)

public:
    AudioClip();
    virtual ~AudioClip();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    bool LoadFromFile(const Path &filepath);

    int GetChannels() const;
    int GetBufferSize() const;
    int GetBitDepth() const;
    int GetFrequency() const;
    float GetLength() const;
    bool IsLoaded() const;

    const Path &GetSoundFilepath() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void OnAudioSourceAttached(AudioSource *as);
    void OnAudioSourceDettached(AudioSource *as);

private:
    ALuint m_alBufferId = 0;
    Path m_audioFileFilepath;
    List<AudioSource*> m_audioSourcesUsingThis;

    ALuint GetALBufferId() const;

    friend class AudioSource;
    friend class AudioPlayerRunnable;
};

#endif // AUDIOCLIP_H
