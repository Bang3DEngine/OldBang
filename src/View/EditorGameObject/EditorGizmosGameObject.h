#ifndef EDITORGIZMOSGAMEOBJECT_H
#define EDITORGIZMOSGAMEOBJECT_H

#include "GameObject.h"

class EditorGizmosGameObject : public GameObject
{
public:
    EditorGizmosGameObject(const String &name = "");

    static EditorGizmosGameObject *GetInstance();

    void Init();

    void SetResetAllowed(bool allowed);

    void SetColor(const Color &color);
    void SetRotation(const Quaternion &rotation);
    void SetPosition(const Vector3 &position);
    void SetScale(const Vector3 &scale);
    void SetLineWidth(float lineWidth);
    void SetDrawWireframe(bool wireframe);
    void SetReceivesLighting(bool receivesLighting);
    void SetScreenSpaceMode(bool screenSpaceMode = true);
    void SetBillboard();
    void RenderCustomMesh(Mesh *m);
    void RenderSimpleBox(const AABox &b);
    void RenderBox(const AABox &b);
    void RenderRect(const Rect &r);
    void RenderCircle(float radius);
    void RenderIcon(const Texture2D *texture, bool billboard);
    void RenderScreenIcon(const Texture2D *texture, const Rect &screenRect);
    void RenderLine(const Vector3 &origin, const Vector3 &destiny);
    void RenderScreenLine(const Vector2 &origin, const Vector2 &destiny);
    void RenderRay(const Vector3 &origin, const Vector3 &rayDir);
    void RenderSphere(const Vector3 &origin, float radius);
    void RenderFrustum(const Vector3 &forward, const Vector3 &up,
                       const Vector3 &origin, float zNear,
                       float zFar, float fovDegrees, float aspectRatio);
    void RenderSimpleSphere(const Vector3 &origin, float radius);
    void Reset();
    void Render(Renderer *rend);

private:
    Mesh *m_boxMesh = nullptr;
    Mesh *m_sphereMesh = nullptr;
    Mesh *m_planeMesh = nullptr;
    Material *m_material = nullptr;

    bool m_inited = false;
    bool m_resetAllowed = true;

    bool IsGizmoRenderer(Renderer *rend);

    friend class Gizmos;
};

#endif // EDITORGIZMOSGAMEOBJECT_H
