#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "Camera.h"
#include "EditorEntity.h"

//ENTITY Camera that you can control in the editor
class EditorCamera : public EditorEntity
{
private:
    Camera *cam = nullptr;

public:
    EditorCamera();
    virtual ~EditorCamera();
};

#endif // EDITORCAMERA_H
