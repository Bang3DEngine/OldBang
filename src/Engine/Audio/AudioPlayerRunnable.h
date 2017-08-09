#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include <QRunnable>

#include "Bang.h"

FORWARD class AudioClip;
FORWARD class ALAudioSource;

class AudioPlayerRunnable : public QRunnable
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

    void run() override;

private:
    AudioClip *m_audioClip = nullptr;
    ALAudioSource *m_alAudioSource = 0;
    float m_delayInSeconds = 0.0f;
};

#endif // AUDIOPLAYERRUNNABLE_H
