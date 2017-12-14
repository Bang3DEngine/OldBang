#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include "Bang/Set.h"
#include "Bang/Time.h"
#include "Bang/Color.h"
#include "Bang/Vector3.h"
#include "Bang/GameObject.h"

NAMESPACE_BANG_BEGIN

FORWARD class LineRenderer;

class DebugRenderer : public GameObject
{
    GAMEOBJECT(DebugRenderer);

public:
    static void RenderLine(const Vector3 &origin,
                           const Vector3 &end,
                           const Color &color = Color::Green,
                           float time = 1.0f,
                           float thickness = 1.0f,
                           bool depthTest = false);

private:
    struct DebugRenderLine
    {
        Vector3 origin;
        Vector3 end;
        Color color;
        float thickness;
        double destroyTimestamp;
        bool depthTest;
        bool renderedOnce;
    };

    List<DebugRenderLine> m_linesToRender;

	DebugRenderer();
	virtual ~DebugRenderer();

    void Render(bool withDepth);

    static DebugRenderer *GetActive();

    friend class Scene;
};

NAMESPACE_BANG_END

#endif // DEBUGRENDERER_H

