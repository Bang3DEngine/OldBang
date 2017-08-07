#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Bang/Path.h"
#include "Bang/List.h"
#include "Bang/Asset.h"

FORWARD class AudioSource;

class AudioClip : public Asset
{
    SOBJECT(AudioClip)

public:
    AudioClip();
    virtual ~AudioClip();

    bool LoadFromFile(const Path &filepath);
    void OnAudioSourceAttached(AudioSource *as);
    void OnAudioSourceDettached(AudioSource *as);

    int GetChannels() const;
    int GetBufferSize() const;
    int GetBitDepth() const;
    int GetFrequency() const;
    float GetLength() const;
    bool IsLoaded() const;
    const Path &GetSoundFilepath() const;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    ALuint m_alBufferId = 0;
    Path m_audioFileFilepath;
    List<AudioSource*> m_audioSourcesUsingThis;

    ALuint GetALBufferId() const;

    friend class AudioSource;
    friend class AudioPlayerRunnable;
};

#endif // AUDIOCLIP_H
