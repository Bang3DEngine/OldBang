#ifndef SCREEN_H
#define SCREEN_H

#include <GL/glew.h>

#include "Bang/Bang.h"
#include "Bang/Screen.h"
#include "Bang/Vector2.h"

FORWARD class GraphicPipeline;

class Screen
{
public:
    Screen();
    virtual ~Screen();

    void Initialize();
    void OnResize(int w, int h);
    void Render() const;

    float GetAspectRatio() const;
    int GetHeight() const;
    int GetWidth() const;
    const Vector2i& GetSize() const;
    Vector2 GetPixelClipSize() const;

    static float GetAspectRatioS();
    static int GetHeightS();
    static int GetWidthS();
    static const Vector2i& GetSizeS();
    static Vector2 GetPixelClipSizeS();
    static Screen *GetInstance();
    GraphicPipeline *GetGraphicPipeline() const;

protected:
    Vector2i m_size = Vector2i::One;

    GraphicPipeline *m_gPipeline = nullptr;
};

#endif // SCREEN_H
