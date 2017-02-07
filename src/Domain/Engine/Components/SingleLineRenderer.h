#ifndef SINGLELINERENDERER_H
#define SINGLELINERENDERER_H

#include "Array.h"

#include "LineRenderer.h"

class SingleLineRenderer : public LineRenderer
{
friend class GameObject;

protected:
    SingleLineRenderer();

public:
    virtual ~SingleLineRenderer();

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void SetOrigin(const Vector3 &o);
    void SetDestiny(const Vector3 &d);

    Vector3 GetOrigin() const;
    Vector3 GetDestiny() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // SINGLELINERENDERER_H
