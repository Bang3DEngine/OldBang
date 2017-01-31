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
    Scene *m_latestSceneBeforePlaying = nullptr;

    bool PlayScene();
    void StopScene();

    bool WaitForAllBehavioursToBeLoaded();

private slots:
    void OnWaitingForBehavioursCanceled();

    friend class EditorState;
};

#endif // EDITORPLAYSTOPFLOWCONTROLLER_H
