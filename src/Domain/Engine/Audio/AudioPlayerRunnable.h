#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include <QRunnable>

class AudioClip;
class AudioPlayerRunnable : public QRunnable
{
public:
    AudioPlayerRunnable(AudioClip *clip, float delaySeconds);

    void run() override;

private:
    AudioClip *m_audioClip = nullptr;
    float m_delaySeconds = 0.0f;
};

#endif // AUDIOPLAYERRUNNABLE_H
