#ifndef EDITORPLAYSTOPFLOWCONTROLLER_H
#define EDITORPLAYSTOPFLOWCONTROLLER_H

#include "Scene.h"

class EditorPlayStopFlowController
{
public:
    EditorPlayStopFlowController();

    static bool IsPlaying();
    static void OnPlayClicked();
    static void OnStopClicked();

    static EditorPlayStopFlowController* GetInstance();

private:
    bool m_playing = false;
    Scene *m_latestSceneBeforePlaying = nullptr;

    void PlayScene();
    void StopScene();
};

#endif // EDITORPLAYSTOPFLOWCONTROLLER_H
