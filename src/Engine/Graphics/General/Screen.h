#ifndef SCREEN_H
#define SCREEN_H

#include <GL/glew.h>

#include "Bang/G_Screen.h"

FORWARD class GraphicPipeline;

class Screen: public G_Screen
{

public:
    Screen();
    virtual ~Screen();

    virtual void Initialize() override;
    virtual void OnResize(int w, int h) override;
    virtual void Render() const;

    static float GetAspectRatio();
    static int GetHeight();
    static int GetWidth();
    static Vector2i GetSize();
    static Vector2 GetPixelClipSize();

    static Screen *GetInstance();
    GraphicPipeline *GetGraphicPipeline() const;

protected:
    GraphicPipeline *m_gPipeline = nullptr;
};

#endif // SCREEN_H
