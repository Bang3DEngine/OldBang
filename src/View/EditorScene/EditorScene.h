#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"

class EditorFloor;
class EditorCamera;
class EditorDebugGameObject;
class EditorTranslateAxisGroup;
class EditorScene : public Scene
{
friend class Debug;

private:
    EditorDebugGameObject *m_debugGameObject = nullptr;
    EditorCamera *m_cameraGameObject = nullptr;
    EditorTranslateAxisGroup *m_axises = nullptr;
    EditorFloor *m_floor = nullptr;

public:
    EditorScene();
    virtual ~EditorScene();

    virtual void _OnStart () override;

    bool IsEditorGameObject() const override;
};

#endif // EDITORSCENE_H
