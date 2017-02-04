#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThreadPool>

class AudioClip;
class AudioManager
{
public:
    static AudioManager *GetInstance();

    static void PlayAudioClip(AudioClip *audioClip, float delaySeconds);

private:
    AudioManager();
    virtual ~AudioManager();

    QThreadPool m_threadPool;

    friend class Application;
};

#endif // AUDIOMANAGER_H
