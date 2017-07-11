#include "Bang/AttrWidgetVectorFloat.h"

AttrWidgetVectorFloat::AttrWidgetVectorFloat(const XMLAttribute &xmlAttribute) :
    AttributeWidget(xmlAttribute, false, true, true)
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    m_horizontalLayout.addLayout(hLayout, 1);
    m_horizontalLayout.setSpacing(0);
    m_horizontalLayout.setMargin(0);

    String labels[] = {"X", "Y", "Z", "W"};
    int numberOfFields = xmlAttribute.GetNumberOfFieldsOfType();
    for (uint i = 0; i < numberOfFields; ++i)
    {
        AttrWidgetFloat *s = new AttrWidgetFloat(xmlAttribute, true);
        m_floatSlots.PushBack(s);

        QObject::connect(s, SIGNAL(OnValueChanged()),
                         this, SIGNAL(OnValueChanged()));

        QLabel *label = new QLabel(labels[i].ToQString());
        if (i != 0)
        {
            hLayout->setSpacing(3);
        }
        hLayout->addWidget(label, 0, Qt::AlignRight | Qt::AlignVCenter);
        hLayout->addWidget(s,     0, Qt::AlignLeft | Qt::AlignVCenter);
    }

    setMinimumWidth(40);
    setFixedHeight(20);
    AfterConstructor();
}

void AttrWidgetVectorFloat::SetValue(const Array<float> &v)
{
    for (uint i = 0; i < m_floatSlots.Size(); ++i)
    {
        m_floatSlots[i]->SetValue(v[i]);
    }
}

Array<float>  AttrWidgetVectorFloat::GetValue() const
{
    Array<float> result;
    for (uint i = 0; i < m_floatSlots.Size(); ++i)
    {
        float f = m_floatSlots[i]->GetValue();
        result.PushBack(f);
    }
    return result;
}

float AttrWidgetVectorFloat::GetFloat() const
{
    return m_floatSlots[0]->GetValue();
}

Vector2 AttrWidgetVectorFloat::GetVector2() const
{
    return Vector2(m_floatSlots[0]->GetValue(),
                   m_floatSlots[1]->GetValue());
}

Vector3 AttrWidgetVectorFloat::GetVector3() const
{
    return Vector3(m_floatSlots[0]->GetValue(),
                   m_floatSlots[1]->GetValue(),
                   m_floatSlots[2]->GetValue());
}

Vector4 AttrWidgetVectorFloat::GetVector4() const
{
    return Vector4(m_floatSlots[0]->GetValue(),
                   m_floatSlots[1]->GetValue(),
                   m_floatSlots[2]->GetValue(),
                   m_floatSlots[3]->GetValue());
}

void AttrWidgetVectorFloat::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);

    XMLAttribute::Type attrType = attribute.GetType();

    Array<float> vf;
    if (attrType == XMLAttribute::Type::Float)
    {
        float v = attribute.GetFloat();
        vf = {v};
    }
    else if (attrType == XMLAttribute::Type::Vector2)
    {
        Vector2 v = attribute.GetVector2();
        vf = {v.x, v.y};
    }
    else if (attrType == XMLAttribute::Type::Vector3)
    {
        Vector3 v = attribute.GetVector3();
        vf = {v.x, v.y, v.z};
    }
    else if (attrType == XMLAttribute::Type::Vector4 ||
             attrType == XMLAttribute::Type::Quaternion ||
             attrType == XMLAttribute::Type::Rect)
    {
        Vector4 v = attribute.GetVector4();
        vf = {v.x, v.y, v.z, v.w};
    }

    SetValue(vf);
}

XMLAttribute AttrWidgetVectorFloat::GetXMLAttribute() const
{
    XMLAttribute attr;
    Array<float> floats = GetValue();
    if (floats.Size() == 2)
    {
        attr.SetVector2( Vector2(floats[0], floats[1]) );
    }
    else if (floats.Size() == 3)
    {
        attr.SetVector3( Vector3(floats[0], floats[1], floats[2]));
    }
    else if (floats.Size() == 4)
    {
        attr.SetVector4( Vector4(floats[0], floats[1], floats[2], floats[3]) );
    }
    return attr;
}
