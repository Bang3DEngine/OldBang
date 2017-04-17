#ifndef EDITORPLAYFLOW_H
#define EDITORPLAYFLOW_H

#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/Scene.h"
#include "Bang/EditorState.h"

class EditorPlayFlow : public QObject
{
    Q_OBJECT

public:
    EditorPlayFlow();

    static bool OnPlayClicked();
    static void OnPauseClicked();
    static void OnStepFrameClicked();
    static void OnStopClicked();

    static void OnFrameFinished();

    static EditorPlayFlow* GetInstance();

private:
    bool m_stepNextFrame   = false;
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

#endif // EDITORPLAYFLOW_H
