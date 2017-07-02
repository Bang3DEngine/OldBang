#ifndef CIRCLECULLEDRENDERER_H
#define CIRCLECULLEDRENDERER_H

#include "Bang/LineRenderer.h"

class CircleCulledRenderer : public LineRenderer
{
public:
    CircleCulledRenderer();
    float GetRadius() const;
    void OnUpdate() override;

    void GetTwoClosestPointsInScreenSpace(
            const Vector2 &sOrigin,
            Vector2 *p0, int *i0,
            Vector2 *p1, int *i1 ) const;

protected:
    virtual void ReloadPoints();
    Vector3 GetCircleLocalPoint(float angle) const;
};

#endif // CIRCLECULLEDRENDERER_H
