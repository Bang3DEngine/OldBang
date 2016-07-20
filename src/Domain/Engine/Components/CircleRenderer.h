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
            const glm::vec2 &sOrigin,
            int pointIndex,
            const Matrix4 &modelViewProjMatrix) const;

    // Used by EditorRotateAxis mainly
    void GetTwoClosestPointsInScreenSpace(
            const glm::vec2 &sOrigin,
            const Matrix4 &modelViewProjMatrix,
            glm::vec2 *p0, int *i0,
            glm::vec2 *p1, int *i1 ) const;

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
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;
#endif

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // CIRCLERENDERER_H
