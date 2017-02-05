#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include <QRunnable>

#include "Vector3.h"
#include "AudioPlayProperties.h"

class AudioClip;
class AudioPlayerRunnable : public QRunnable
{
public:
    AudioPlayerRunnable(AudioClip *clip, const AudioPlayProperties &properties);

    void run() override;

private:
    AudioClip *m_audioClip = nullptr;
    AudioPlayProperties m_properties;
};

#endif // AUDIOPLAYERRUNNABLE_H
