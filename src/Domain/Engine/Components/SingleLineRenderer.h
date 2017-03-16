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
    ICloneable *CloneVirtual() const override;

    void SetOrigin(const Vector3 &o);
    void SetDestiny(const Vector3 &d);

    Vector3 GetOrigin() const;
    Vector3 GetDestiny() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    friend class ICloneable;
};

#endif // SINGLELINERENDERER_H
