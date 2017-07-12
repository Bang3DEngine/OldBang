#include "Bang/AttrWidgetVectorFloat.h"

AttrWidgetVectorFloat::AttrWidgetVectorFloat(const String &labelText) :
    AttributeWidget()
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    m_horizontalLayout.addLayout(hLayout, 1);
    m_horizontalLayout.setSpacing(0);
    m_horizontalLayout.setMargin(0);

    String labels[] = {"X", "Y", "Z", "W"};
    int numberOfFields = 4;
    for (uint i = 0; i < numberOfFields; ++i)
    {
        AttrWidgetFloat *s = new AttrWidgetFloat(labels[i]);
        m_floatSlots.PushBack(s);

        QObject::connect(s, SIGNAL(OnValueChanged(AttributeWidget*)),
                         this, SLOT(OnValueChanged(AttributeWidget*)));

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

    CreateLabel(labelText);
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

void AttrWidgetVectorFloat::Refresh()
{
    AttributeWidget::Refresh();
}
