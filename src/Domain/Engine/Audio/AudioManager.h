#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThreadPool>

#include "AudioPlayProperties.h"

class AudioClip;
class AudioManager
{
public:
    static AudioManager *GetInstance();

    static void PlayAudioClip(AudioClip *audioClip,
                              const AudioPlayProperties &audioPlayProperties);

private:
    AudioManager();
    virtual ~AudioManager();

    QThreadPool m_threadPool;

    friend class Application;
};

#endif // AUDIOMANAGER_H
