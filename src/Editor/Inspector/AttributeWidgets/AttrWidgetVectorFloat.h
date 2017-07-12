#ifndef ATTRWIDGETVECTORFLOAT_H
#define ATTRWIDGETVECTORFLOAT_H

#include "Bang/Array.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Quaternion.h"

#include "Bang/AttrWidgetFloat.h"

 //Slot for a vector of size N
class AttrWidgetVectorFloat : public AttributeWidget
{
    Q_OBJECT

public:
    Array<AttrWidgetFloat*> m_floatSlots;

    AttrWidgetVectorFloat(const String &labelText, int numFields);
    virtual ~AttrWidgetVectorFloat();

    virtual void SetValue(const Array<float> &v);
    virtual void SetValue(const Vector2 &v);
    virtual void SetValue(const Vector3 &v);
    virtual void SetValue(const Vector4 &v);
    virtual Array<float> GetValue() const;
    float GetFloat() const;
    Vector2 GetVector2() const;
    Vector3 GetVector3() const;
    Vector4 GetVector4() const;

    virtual void Refresh() override;

private slots:
    void OnFloatValueChanged(IAttributeWidget *attrWidget);
};

#endif // ATTRWIDGETVECTORFLOAT_H
