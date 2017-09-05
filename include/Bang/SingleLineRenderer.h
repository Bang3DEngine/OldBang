#ifndef SINGLELINERENDERER_H
#define SINGLELINERENDERER_H

#include "Bang/LineRenderer.h"

class SingleLineRenderer : public LineRenderer
{
    COMPONENT(SingleLineRenderer)

public:
    virtual ~SingleLineRenderer();

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
};

#endif // SINGLELINERENDERER_H
