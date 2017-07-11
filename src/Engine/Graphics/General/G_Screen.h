#ifndef G_SCREEN_H
#define G_SCREEN_H

#include <GL/glew.h>
#include <chrono>

#include "Bang/Array.h"
#include "Bang/Vector2.h"

class G_Screen
{
public:
    explicit G_Screen();
    virtual ~G_Screen();

    void OnInitialize();
    void OnResize(int w, int h);
    virtual void Render();

    float GetAspectRatio() const;
    int GetHeight() const;
    int GetWidth() const;
    Vector2 GetSize() const;
    Vector2 GetPixelClipSize() const;

protected:
    int m_width = 1;
    int m_height = 1;
};

#endif // G_SCREEN_H
