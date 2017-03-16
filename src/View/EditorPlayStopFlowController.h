#ifndef EDITORPLAYSTOPFLOWCONTROLLER_H
#define EDITORPLAYSTOPFLOWCONTROLLER_H

#include <QObject>

#include "Scene.h"

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

    bool WaitForAllBehavioursToBeLoaded();

private slots:
    bool PlayScene();
    void StopScene();

    void OnWaitingForBehavioursCanceled();

    friend class EditorState;
};

#endif // EDITORPLAYSTOPFLOWCONTROLLER_H
