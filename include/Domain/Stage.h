#ifndef STAGE_H
#define STAGE_H

#include <list>
#include <string>

#include "Logger.h"
#include "Entity.h"
#include "Camera.h"
#include "Canvas.h"
#include "Framebuffer.h"
#include "MeshPlane.h"
#include "Material.h"
#include "StageEventListener.h"

class Canvas;
class Stage : public Entity
{
friend class Canvas;
public:
    enum GeometryFBAttachment
    {
        Position = 0,
        Normal,
        Uv,
        Diffuse
    };

private:
    //Simple material with pass by shaders
    //When you want to render with it, just attach
    //the final color texture to texture slot 0
    Material *renderToScreenMaterial;
    Mesh *planeMesh; //This will be used to render to screen

protected:
    Stage();

    //Framebuffer for positions, normals, uvs and diffuse
    Framebuffer *geometryFramebuffer;

    const Canvas *canvas;
    const Entity *cameraEntity;

public:
    virtual ~Stage();

    void _OnRender() override;

    void SetCameraEntity(const Entity *cameraEntity);

    Camera* GetCamera() const;
    const Canvas* GetCanvas() const;
};

#endif // STAGE_H
