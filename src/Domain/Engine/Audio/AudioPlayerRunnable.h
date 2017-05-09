#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include <QRunnable>
#include "Bang/WinUndef.h"

#include "Bang/Vector3.h"

class AudioClip;
class AudioPlayerRunnable : public QRunnable
{
public:
    AudioPlayerRunnable(AudioClip *clip, int alSourceId,
                        float delayInSeconds = 0.0f);

    void Resume();
    void Pause();
    void Stop();
    void OnAudioManagerDelete();

    void run() override;

private:
    AudioClip *m_audioClip = nullptr;
    float m_delayInSeconds = 0.0f;
    int m_alSourceId = 0;

    volatile bool m_paused  = false;
    volatile bool m_stopped = false;
    volatile bool m_exited  = false;
};

#endif // AUDIOPLAYERRUNNABLE_H
