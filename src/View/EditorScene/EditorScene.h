#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Scene.h"
#include "EditorFloor.h"
#include "EditorCamera.h"
#include "SelectionFramebuffer.h"
#include "EditorDebugGameObject.h"
#include "EditorTranslateAxisGroup.h"

class EditorScene : public Scene
{
friend class Debug;

private:
    EditorDebugGameObject *m_debugGameObject = nullptr;
    EditorCamera *m_cameraGameObject = nullptr;
    EditorTranslateAxisGroup *m_axises = nullptr;
    EditorFloor *m_floor = nullptr;

    SelectionFramebuffer *m_selectionFramebuffer = nullptr;

public:
    EditorScene();
    virtual ~EditorScene();

    virtual void _OnStart () override;
    virtual void _OnResize (int newWidth, int newHeight) override;
    virtual void _OnRender () override;

    SelectionFramebuffer* GetSelectionFramebuffer() const;

    bool IsEditorGameObject() const override;
};

#endif // EDITORSCENE_H
