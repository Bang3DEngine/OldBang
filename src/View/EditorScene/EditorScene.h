#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"

class UIText;
class EditorFloor;
class EditorCamera;
class EditorDebugGameObject;
class EditorGizmosGameObject;
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

    bool IsEditorGameObject() const override;

    void SetEditorCamera();
    Camera *GetCamera() const override;
    EditorCamera *GetEditorCamera() const;

    EditorGizmosGameObject *GetGizmosGameObject() const;

private:
    EditorGizmosGameObject *m_gizmosGameObject = nullptr;
    EditorDebugGameObject *m_debugGameObject = nullptr;
    EditorCamera *m_edCameraGameObject = nullptr;
    EditorFloor *m_floor = nullptr;

    EditorGameObject *m_fpsCounter = nullptr;
    UIText *m_fpsCounterText = nullptr;

    friend class Debug;
};

#endif // EDITORSCENE_H
