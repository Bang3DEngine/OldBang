#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Asset.h"
#include "String.h"
#include "Persistence.h"

class AudioClip : public Asset
{
public:
    enum State
    {
        Playing = AL_PLAYING,
        Paused  = AL_PAUSED,
        Stopped = AL_STOPPED
    };

    AudioClip();
    virtual ~AudioClip();

    const static String GetFileExtensionStatic();
    const virtual String GetFileExtension();

    void LoadFromFile(const String &filepath);

    void Play();
    void Pause();
    void Stop();

    bool IsLoaded() const;

    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsStopped() const;

    State GetState() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

private:
    ALuint m_alBufferId = 0;
    ALuint m_alSourceId = 0;

    ALuint GetALBufferId() const;
    ALuint GetALSourceId() const;

    void Free();

    friend class AudioPlayerRunnable;
};

#endif // AUDIOCLIP_H
