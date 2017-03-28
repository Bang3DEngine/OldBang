#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include <QRunnable>

#include "Bang/Vector3.h"

class AudioClip;
class AudioPlayerRunnable : public QRunnable
{
public:
    AudioPlayerRunnable(AudioClip *clip, int alSourceId,
                        float delayInSeconds = 0.0f);

    void run() override;

private:
    AudioClip *m_audioClip = nullptr;
    int m_alSourceId = 0;
    float m_delayInSeconds = 0.0f;
};

#endif // AUDIOPLAYERRUNNABLE_H
