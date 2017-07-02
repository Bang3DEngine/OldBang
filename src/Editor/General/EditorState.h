#ifndef EDITORSTATE_H
#define EDITORSTATE_H

class EditorState
{
public:
    enum TransformMode
    {
        Translate, Rotate, Scale, RectTransform
    };

    enum PlayState
    {
        Playing, Paused, Stopped
    };

    EditorState();

    static bool IsPlaying();
    static bool IsPaused();
    static bool IsStopped();
    static bool IsUsingGlobalCoords();
    static bool ShowGizmosEnabled();
    static bool ShowGizmosSceneEnabled();
    static bool ShowGizmosGameEnabled();
    static TransformMode GetCurrentTransformMode();
    static bool IsShowingGameTab();
    static bool IsShowingSceneTab();

    static PlayState GetPlayState();

    static void SetTransformMode(TransformMode transformMode);

private:
    static EditorState *GetInstance();

    bool m_globalCoords    = false;
    bool m_showGizmosScene = true;
    bool m_showGizmosGame  = false;
    EditorState::PlayState m_playState   = EditorState::Stopped;
    TransformMode m_currentTransformMode = TransformMode::Translate;

    static void SetPlayingState(PlayState playingState);

    friend class Toolbar;
    friend class EditorPlayFlow;
};

#endif // EDITORSTATE_H
