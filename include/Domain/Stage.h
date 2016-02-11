#ifndef STAGE_H
#define STAGE_H

#include <list>
#include <string>

#include "Logger.h"
#include "Entity.h"
#include "Camera.h"
#include "Canvas.h"
#include "StageEventListener.h"

class Canvas;
class Stage : public Entity
{
friend class Canvas;
protected:
    Stage();

    const Canvas *canvas;
    const Entity *cameraEntity;

public:
    virtual ~Stage();

    void SetCameraEntity(const Entity *cameraEntity);
    Camera* GetCamera() const;

    const Canvas* GetCanvas() const;
};

#endif // STAGE_H
