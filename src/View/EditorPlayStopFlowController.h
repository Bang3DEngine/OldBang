#ifndef EDITORPLAYSTOPFLOWCONTROLLER_H
#define EDITORPLAYSTOPFLOWCONTROLLER_H

#include "Scene.h"

class EditorPlayStopFlowController
{
public:
    EditorPlayStopFlowController();

    static bool OnPlayClicked();
    static void OnStopClicked();

    static EditorPlayStopFlowController* GetInstance();

private:
    bool m_playing = false;
    Scene *m_latestSceneBeforePlaying = nullptr;

    bool PlayScene();
    void StopScene();

    friend class EditorState;
};

#endif // EDITORPLAYSTOPFLOWCONTROLLER_H
