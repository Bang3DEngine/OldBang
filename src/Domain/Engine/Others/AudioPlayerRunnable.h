#ifndef AUDIOPLAYERRUNNABLE_H
#define AUDIOPLAYERRUNNABLE_H

#include <QRunnable>

class AudioClip;
class AudioPlayerRunnable : public QRunnable
{
public:
    AudioPlayerRunnable(AudioClip *clip);

    void run() override;

private:
    AudioClip *m_audioClip = nullptr;
};

#endif // AUDIOPLAYERRUNNABLE_H
