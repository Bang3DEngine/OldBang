#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Material.h"
#include "Part.h"

class Renderer : public Part
{
protected:
    Material *material = nullptr;

    virtual void Render() const = 0;
    virtual void _OnRender() override = 0;

public:
    Renderer();

    virtual void SetMaterial(Material *m) = 0;
    virtual Material* GetMaterial();

    virtual const std::string ToString() const override { return "Renderer"; }
    virtual std::string GetName() const override { return "Renderer"; }
};

#endif // RENDERER_H
