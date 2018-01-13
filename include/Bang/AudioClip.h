#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>

#include "Bang/Path.h"
#include "Bang/List.h"
#include "Bang/Asset.h"

NAMESPACE_BANG_BEGIN

FORWARD class AudioSource;

class AudioClip : public Asset
{
    ASSET(AudioClip)

public:
    int GetChannels() const;
    int GetBufferSize() const;
    int GetBitDepth() const;
    int GetFrequency() const;
    float GetLength() const;
    bool IsLoaded() const;
    const Path &GetSoundFilepath() const;

    // Resource
    void Import(const Path &soundFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    ALuint m_alBufferId = 0;
    Path m_soundFilepath;

    AudioClip();
    virtual ~AudioClip();

    void FreeBuffer();
    ALuint GetALBufferId() const;

    friend class AudioSource;
    friend class AudioManager;
    friend class AudioPlayerRunnable;
};

NAMESPACE_BANG_END

#endif // AUDIOCLIP_H
