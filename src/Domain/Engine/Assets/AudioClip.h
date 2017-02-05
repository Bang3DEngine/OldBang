#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Asset.h"
#include "String.h"
#include "Persistence.h"
#include "AudioPlayProperties.h"

class AudioClip : public Asset
{
public:
    AudioClip();
    virtual ~AudioClip();

    const static String GetFileExtensionStatic();
    const virtual String GetFileExtension();

    void LoadFromFile(const String &filepath);

    void Play();
    void Pause();
    void Stop();

    bool IsLoaded() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

private:
    ALuint m_tempSourceId = 0;
    ALuint m_alBufferId = 0;

    ALuint GetALBufferId() const;

    void Free();

    friend class AudioSource;
    friend class AudioPlayerRunnable;
};

#endif // AUDIOCLIP_H
