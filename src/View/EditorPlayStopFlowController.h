#ifndef EDITORPLAYSTOPFLOWCONTROLLER_H
#define EDITORPLAYSTOPFLOWCONTROLLER_H

#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/Scene.h"

class EditorPlayStopFlowController : public QObject
{
    Q_OBJECT

public:
    EditorPlayStopFlowController();

    static bool OnPlayClicked();
    static void OnStopClicked();

    static EditorPlayStopFlowController* GetInstance();

private:
    bool m_playing = false;
    bool m_playingCanceled = false;
    Scene *p_latestSceneBeforePlaying = nullptr;
    String m_latestSceneBeforePlayingFilepath = "";

    bool WaitForAllBehavioursToBeLoaded();

private slots:
    bool PlayScene();
    void StopScene();

    void OnWaitingForBehavioursCanceled();

    friend class EditorState;
};

#endif // EDITORPLAYSTOPFLOWCONTROLLER_H
