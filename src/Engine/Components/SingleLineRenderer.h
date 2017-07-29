#ifndef SINGLELINERENDERER_H
#define SINGLELINERENDERER_H

#include "Bang/LineRenderer.h"

FORWARD_T class Vector3G;

class SingleLineRenderer : public LineRenderer
{
    OBJECT(SingleLineRenderer)

public:
    virtual ~SingleLineRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    void SetOrigin(const Vector3G<float> &o);
    void SetDestiny(const Vector3G<float> &d);

    Vector3G<float> GetOrigin() const;
    Vector3G<float> GetDestiny() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    SingleLineRenderer();
};

#endif // SINGLELINERENDERER_H
