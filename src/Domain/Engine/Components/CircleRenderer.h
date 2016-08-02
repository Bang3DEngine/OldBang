#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include <vector>

#include "VBO.h"
#include "VAO.h"
#include "Mesh.h"
#include "Component.h"
#include "Scene.h"
#include "Material.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "ShaderContract.h"
#include "AssetsManager.h"
#include "FileReader.h"
#include "ShaderContract.h"

#include "LineRenderer.h"

class CircleRenderer : public LineRenderer
{

friend class EditorRotateAxis;

private:

    float m_radius = 1.0f;
    int m_segments = 32;

    void GeneratePoints();

    // Used by EditorRotateAxis mainly
    float GetDistanceInScreenSpace(
            const Vector2 &sOrigin,
            int pointIndex,
            const Matrix4 &modelViewProjMatrix) const;

    // Used by EditorRotateAxis mainly
    void GetTwoClosestPointsInScreenSpace(
            const Vector2 &sOrigin,
            const Matrix4 &modelViewProjMatrix,
            Vector2 *p0, int *i0,
            Vector2 *p1, int *i1 ) const;

public:
    CircleRenderer();
    virtual ~CircleRenderer();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void SetRadius(float m_radius);
    void SetSegments(int m_segments);

    float GetRadius() const;
    int GetSegments() const;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // CIRCLERENDERER_H
