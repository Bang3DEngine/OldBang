#ifndef STAGE_H
#define STAGE_H

#include "Bang.h"

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
    GBuffer *gbuffer = nullptr;

    const Canvas *canvas = nullptr; //set by canvas when added to it
    const Entity *cameraEntity = nullptr;

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
