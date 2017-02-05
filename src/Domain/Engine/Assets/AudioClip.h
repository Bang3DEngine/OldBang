#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Array.h"
#include "Asset.h"
#include "String.h"
#include "Persistence.h"
#include "AudioPlayProperties.h"

class AudioSource;
class AudioClip : public Asset
{
public:
    AudioClip();
    virtual ~AudioClip();

    const static String GetFileExtensionStatic();
    const virtual String GetFileExtension();

    bool LoadFromFile(const String &filepath);

    void Play();
    void Pause();
    void Stop();

    int GetChannels() const;
    int GetBufferSize() const;
    int GetBitDepth() const;
    int GetFrequency() const;
    float GetLength() const;
    bool IsLoaded() const;

    const String &GetAudioFilepath() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    void OnAudioSourceAttached(AudioSource *as);
    void OnAudioSourceDettached(AudioSource *as);

private:
    ALuint m_alBufferId = 0;
    String m_audioFileFilepath = "";
    List<AudioSource*> m_audioSourcesUsingThis;

    ALuint GetALBufferId() const;

    friend class AudioSource;
    friend class AudioPlayerRunnable;
};

#endif // AUDIOCLIP_H
