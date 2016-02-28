#include "InspectorVectorFloatPartSlotWidget.h"

InspectorVectorFloatPartSlotWidget::InspectorVectorFloatPartSlotWidget(std::vector<float> initialValues,
                                                            const std::string &labelString,
                                                            InspectorPartWidget *parent) : InspectorPartSlotWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(0); vLayout->setContentsMargins(0,0,0,0);
    this->setLayout(vLayout);

    QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
    textLabel->setContentsMargins(0,0,0,0);
    vLayout->addWidget(textLabel);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0); hLayout->setContentsMargins(0,0,0,0);
    vLayout->addLayout(hLayout);

    for(unsigned int i = 0; i < initialValues.size(); ++i)
    {
        InspectorFloatPartSlotWidget *s = new InspectorFloatPartSlotWidget(initialValues[i], "", parent);
        floatSlots.push_back(s);

        s->setContentsMargins(0,0,0,0); s->show();
        hLayout->addWidget(s);
    }

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorVectorFloatPartSlotWidget::SetValue(const std::vector<float> &v)
{
    for(unsigned int i = 0; i < floatSlots.size(); ++i)
    {
        floatSlots[i]->SetValue(v[i]);
    }
}

std::vector<float>  InspectorVectorFloatPartSlotWidget::GetValue()
{
    std::vector<float> result;
    for(unsigned int i = 0; i < floatSlots.size(); ++i)
    {
        float f = floatSlots[i]->GetValue();
        result.push_back(f);
    }
    return result;
}
