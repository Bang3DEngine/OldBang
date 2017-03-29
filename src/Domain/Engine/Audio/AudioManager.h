#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThreadPool>
#include "Bang/WinUndef.h"

class AudioClip;
class GameObject;
class AudioManager
{
public:
    static AudioManager *GetInstance();

    static void PlayAudioClip(AudioClip *audioClip, int alSourceId,
                              float delayInSeconds = 0.0f);

    static void ClearALErrors();
    static bool CheckALError();

private:
    AudioManager();
    virtual ~AudioManager();

    QThreadPool m_threadPool;
    GameObject *m_currentListener = nullptr;

    friend class Application;
};

#endif // AUDIOMANAGER_H
