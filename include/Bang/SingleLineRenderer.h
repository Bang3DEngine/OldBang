#ifndef SINGLELINERENDERER_H
#define SINGLELINERENDERER_H

#include "Bang/LineRenderer.h"

NAMESPACE_BANG_BEGIN

FORWARD_T class Vector3G;

class SingleLineRenderer : public LineRenderer
{
    COMPONENT(SingleLineRenderer)

public:
    void SetOrigin(const Vector3 &o);
    void SetDestiny(const Vector3 &d);

    Vector3 GetOrigin() const;
    Vector3 GetDestiny() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    SingleLineRenderer();
    virtual ~SingleLineRenderer();
};

NAMESPACE_BANG_END

#endif // SINGLELINERENDERER_H
