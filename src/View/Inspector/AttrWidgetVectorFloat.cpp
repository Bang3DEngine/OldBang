#include "AttrWidgetVectorFloat.h"

AttrWidgetVectorFloat::AttrWidgetVectorFloat(const std::string &labelString,
                                             int numberOfFields,
                                             InspectorWidget *parent) :
    AttributeWidget(labelString, parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    setLayout(vLayout);

    vLayout->addWidget(GetLabelWidget(labelString));

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0); hLayout->setContentsMargins(0,0,0,0);
    vLayout->addLayout(hLayout);

    for (unsigned int i = 0; i < numberOfFields; ++i)
    {
        AttrWidgetFloat *s = new AttrWidgetFloat("", parent);
        m_floatSlots.push_back(s);
        hLayout->addWidget(s);
    }

    setContentsMargins(0,0,0,0);
    adjustSize();
    show();
}

void AttrWidgetVectorFloat::SetValue(const std::vector<float> &v)
{
    for (unsigned int i = 0; i < m_floatSlots.size(); ++i)
    {
        m_floatSlots[i]->SetValue(v[i]);
    }
}

std::vector<float>  AttrWidgetVectorFloat::GetValue()
{
    std::vector<float> result;
    for (unsigned int i = 0; i < m_floatSlots.size(); ++i)
    {
        float f = m_floatSlots[i]->GetValue();
        result.push_back(f);
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
