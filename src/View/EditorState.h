#ifndef EDITORSTATE_H
#define EDITORSTATE_H

class EditorState
{
public:
    enum TransformMode
    {
        Translate, Rotate, Scale
    };

    EditorState();

    static bool IsPlaying();
    static bool IsUsingGlobalCoords();
    static bool ShowGizmosEnabled();
    static bool ShowGizmosSceneEnabled();
    static bool ShowGizmosGameEnabled();
    static TransformMode GetCurrentTransformMode();
    static bool IsShowingGameTab();
    static bool IsShowingSceneTab();

private:
    static EditorState *GetInstance();

    bool m_isPlaying       = false;
    bool m_globalCoords    = false;
    bool m_showGizmosScene = true;
    bool m_showGizmosGame  = false;
    TransformMode m_currentTransformMode = TransformMode::Translate;

    friend class Toolbar;
};

#endif // EDITORSTATE_H
