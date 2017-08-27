#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>

#include "Bang/Path.h"
#include "Bang/List.h"
#include "Bang/Asset.h"

FORWARD class AudioSource;

class AudioClip : public Asset
{
    ASSET(AudioClip)

public:
    AudioClip();
    virtual ~AudioClip();

    int GetChannels() const;
    int GetBufferSize() const;
    int GetBitDepth() const;
    int GetFrequency() const;
    float GetLength() const;
    bool IsLoaded() const;
    const Path &GetSoundFilepath() const;

    // Resource
    void Import(const Path &soundFilepath) override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    ALuint m_alBufferId = 0;
    Path m_soundFilepath;

    ALuint GetALBufferId() const;

    friend class AudioSource;
    friend class AudioManager;
    friend class AudioPlayerRunnable;
};

#endif // AUDIOCLIP_H
