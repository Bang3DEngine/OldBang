#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include "Bang/Thread.h"

#include "Bang/IEventEmitter.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class AudioClip;
FORWARD class ALAudioSource;

class AudioPlayerRunnable : public ThreadRunnable,
                            public EventEmitter<IDestroyListener>
{
public:
    AudioPlayerRunnable(AudioClip *clip,
                        ALAudioSource *alAudioSource,
                        float delayInSeconds = 0.0f);
    virtual ~AudioPlayerRunnable();

    void Resume();
    void Pause();
    void Stop();

    AudioClip* GetAudioClip() const;
    ALAudioSource* GetALAudioSource() const;

    void Run() override;

private:
    bool m_forceExit = false;
    AudioClip *p_audioClip = nullptr;
    ALAudioSource *p_alAudioSource = 0;
    float m_delayInSeconds = 0.0f;
};

NAMESPACE_BANG_END

#endif // AUDIOPLAYERRUNNABLE_H
