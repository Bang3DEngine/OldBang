#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Bang/Scene.h"

class UIText;
class EditorFloor;
class EditorCamera;
class EditorDebugGameObject;
class EditorGizmosGameObject;
class EditorTranslateAxisGroup;
class EditorScene : public Scene
{
    ICLONEABLE(EditorScene)

public:
    EditorScene();
    virtual ~EditorScene();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void OnEditorUpdate() override;

    void SetEditorCamera();
    Camera *GetCamera() const override;
    EditorCamera *GetEditorCamera() const;

    static EditorScene *GetNewDefaultScene();

    EditorGizmosGameObject *GetGizmosGameObject() const;

    virtual void PostRead(const XMLNode &xmlInfo) override;

protected:
    virtual void _OnStart() override;

private:
    EditorGizmosGameObject *m_gizmosGameObject = nullptr;
    EditorDebugGameObject *m_debugGameObject = nullptr;
    EditorCamera *m_edCameraGameObject = nullptr;
    EditorFloor *m_floor = nullptr;

    GameObject *m_fpsCounter = nullptr;
    UIText *m_fpsCounterText = nullptr;

    friend class Debug;
};

#endif // EDITORSCENE_H
