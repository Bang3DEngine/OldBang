#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"

class UIText;
class EditorFloor;
class EditorCamera;
class EditorDebugGameObject;
class EditorTranslateAxisGroup;
class EditorScene : public Scene
{

public:
    EditorScene();
    virtual ~EditorScene();

    virtual ICloneable *Clone() const override;
    virtual void CloneInto(ICloneable *clone) const override;

    virtual void _OnStart () override;
    virtual void OnUpdate() override;

    void OnEditorStop();
    void OnEditorPlay();

    bool IsEditorGameObject() const override;

    EditorGameObject *GetGizmosGameObject() const;

private:
    EditorGameObject *m_gizmosGameObject = nullptr;
    EditorDebugGameObject *m_debugGameObject = nullptr;
    EditorCamera *m_edCameraGameObject = nullptr;
    EditorFloor *m_floor = nullptr;

    EditorGameObject *m_fpsCounter = nullptr;
    UIText *m_fpsCounterText = nullptr;

    friend class Debug;
};

#endif // EDITORSCENE_H
