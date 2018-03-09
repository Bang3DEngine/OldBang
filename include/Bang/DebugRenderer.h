#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

#include "Bang/Set.h"
#include "Bang/Mesh.h"
#include "Bang/Time.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/AARect.h"
#include "Bang/Vector3.h"
#include "Bang/GameObject.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD class Quad;
FORWARD class Triangle;
FORWARD class LineRenderer;

class DebugRenderer : public GameObject
{
    GAMEOBJECT(DebugRenderer);

public:
    static void Clear();

    static void RenderLine(const Vector3 &origin,
                           const Vector3 &end,
                           const Color &color = Color::Green,
                           float time = 1.0f,
                           float thickness = 1.0f,
                           bool depthTest = false);

    static void RenderPoint(const Vector3 &point,
                            const Color &color = Color::Green,
                            float time = 1.0f,
                            float thickness = 1.0f,
                            bool depthTest = false);

    static void RenderLineNDC(const Vector2 &originNDC,
                              const Vector2 &endNDC,
                              const Color &color = Color::Green,
                              float time = 1.0f,
                              float thickness = 1.0f,
                              bool depthTest = false);

    static void RenderAABox(const AABox &aaBox,
                            const Color &color = Color::Green,
                            float time = 1.0f,
                            float thickness = 1.0f,
                            bool depthTest = false);

    static void RenderTriangle(const Triangle &triangle,
                               const Color &color = Color::Green,
                               float time = 1.0f,
                               bool wireframe = false,
                               bool culling = false,
                               bool depthTest = false);
    static void RenderQuad(const Quad &quad,
                           const Color &color = Color::Green,
                           float time = 1.0f,
                           bool wireframe = false,
                           bool culling = false,
                           bool depthTest = false);

    static void RenderAARectNDC(const AARect &aaRectNDC,
                                const Color &color = Color::Green,
                                float time = 1.0f,
                                float thickness = 1.0f,
                                bool depthTest = false);

    static void RenderRectNDC(const Rect &rectNDC,
                              const Color &color = Color::Green,
                              float time = 1.0f,
                              float thickness = 1.0f,
                              bool depthTest = false);

private:
    enum class DebugRendererPrimitiveType
    { Point, Line, LineNDC, AARectNDC, RectNDC, Triangle, Quad };

    struct DebugRenderPrimitive
    {
        DebugRendererPrimitiveType primitive;
        Vector3 p0, p1, p2, p3;
        RectPoints rectNDCPoints;
        AARect aaRectNDC;
        Color color;
        float thickness;
        double destroyTimestamp;
        bool wireframe;
        bool culling;
        bool depthTest;
        bool renderedOnce;
    };

    RH<Mesh> m_mesh;
    List<DebugRenderPrimitive> m_primitivesToRender;

	DebugRenderer();
	virtual ~DebugRenderer();

    void RenderPrimitives(bool withDepth);

    static DebugRenderPrimitive*
           CreateDebugRenderPrimitive(DebugRendererPrimitiveType primitive,
                                      const Array<Vector3> &points,
                                      const Color &color,
                                      float time,
                                      float thickness,
                                      bool wireframe,
                                      bool culling,
                                      bool depthTest);

    static DebugRenderer *GetActive();

    friend class Scene;
    friend class GameObject;
};

NAMESPACE_BANG_END

#endif // DEBUGRENDERER_H

