#ifndef SINGLELINERENDERER_H
#define SINGLELINERENDERER_H

#include "Array.h"

#include "LineRenderer.h"

class SingleLineRenderer : public LineRenderer
{
    OBJECT(SingleLineRenderer)
    ICLONEABLE(SingleLineRenderer)

public:
    virtual ~SingleLineRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    void SetOrigin(const Vector3 &o);
    void SetDestiny(const Vector3 &d);

    Vector3 GetOrigin() const;
    Vector3 GetDestiny() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    SingleLineRenderer();
};

#endif // SINGLELINERENDERER_H
