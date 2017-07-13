#include "Bang/AttrWidgetVectorFloat.h"

AttrWidgetVectorFloat::AttrWidgetVectorFloat(const String &labelText,
                                             int numFields)
    : AttributeWidget()
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    m_horizontalLayout.addLayout(hLayout, 1);
    m_horizontalLayout.setSpacing(0);
    m_horizontalLayout.setMargin(0);

    hLayout->setSpacing(3);
    static String labels[] = {"X", "Y", "Z", "W"};
    static Color labelColors[] = {Color(1.0f, 0.3f, 0.3f),
                                  Color(0.0f, 1.0f, 0.0f),
                                  Color(0.4f, 0.4f, 1.0f),
                                  Color(1.0f, 1.0f, 1.0f)};
    for (uint i = 0; i < numFields; ++i)
    {
        AttrWidgetFloat *s = new AttrWidgetFloat(labels[i]);
        s->SetLabelColor(labelColors[i]);
        m_floatSlots.PushBack(s);

        QObject::connect(s, SIGNAL(OnValueChanged(IAttributeWidget*)),
                         this, SLOT(OnFloatValueChanged(IAttributeWidget*)));
        hLayout->addWidget(s, 0, Qt::AlignLeft | Qt::AlignVCenter);
    }

    setMinimumWidth(20);
    SetHeightSizeHint(20);

    SetLabelText(labelText);
}

AttrWidgetVectorFloat::~AttrWidgetVectorFloat()
{

}

void AttrWidgetVectorFloat::SetValue(const Array<float> &v)
{
    for (uint i = 0; i < m_floatSlots.Size(); ++i)
    {
        m_floatSlots[i]->SetValue(v[i]);
    }
}

void AttrWidgetVectorFloat::SetValue(const Vector2 &v)
{
    SetValue( Array<float>({v.x, v.y}) );
}

void AttrWidgetVectorFloat::SetValue(const Vector3 &v)
{
    SetValue( Array<float>({v.x, v.y, v.z}) );
}

void AttrWidgetVectorFloat::SetValue(const Vector4 &v)
{
    SetValue( Array<float>({v.x, v.y, v.z, v.w}) );
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

void AttrWidgetVectorFloat::Refresh()
{
    AttributeWidget::Refresh();
}

void AttrWidgetVectorFloat::OnFloatValueChanged(IAttributeWidget *attrWidget)
{
    emit AttributeWidget::OnValueChanged(this);
}
