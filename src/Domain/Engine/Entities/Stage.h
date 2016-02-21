#ifndef STAGE_H
#define STAGE_H

#include <list>
#include <string>

#include "Logger.h"
#include "Entity.h"
#include "Camera.h"
#include "Canvas.h"
#include "GBuffer.h"
#include "MeshScreenPlane.h"
#include "Material.h"

class Canvas;
class Stage : public Entity
{
friend class Canvas;

protected:

    //Framebuffer for positions, normals, uvs and diffuse
    GBuffer *gbuffer;

    const Canvas *canvas;
    const Entity *cameraEntity;

    void _OnResize(int newWidth, int newHeight);

public:
    Stage();
    virtual ~Stage();

    void _OnRender() override;

    void SetCameraChild(const std::string &cameraChildName);

    Camera* GetCamera() const;
    const Canvas* GetCanvas() const;
};

#endif // STAGE_H
