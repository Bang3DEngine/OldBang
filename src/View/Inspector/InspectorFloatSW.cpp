#include "InspectorFloatSW.h"

InspectorFloatSW::InspectorFloatSW(const std::string &labelString,
                                           float initialValue,
                                           InspectorWidget *parent) :
    InspectorSW(labelString, parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if (labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    p_spinbox = new FloatComponentSlotSpinBox();
    p_spinbox->setValue(initialValue);
    p_spinbox->setAlignment(Qt::AlignHCenter);
    p_spinbox->setMinimum(-9999999.9);
    p_spinbox->setMaximum(9999999.9);
    p_spinbox->setAccelerated(true);
    p_spinbox->setMinimumWidth(50);
    p_spinbox->setContentsMargins(0,0,0,0);
    p_spinbox->show();

    layout->addWidget(p_spinbox);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorFloatSW::SetValue(float f)
{
    if (!_editing)
    {
        disconnect(p_spinbox, SIGNAL(valueChanged(double)), p_parent, SLOT(_OnSlotValueChanged(double)));
        p_spinbox->setValue(f);
        p_spinbox->show();
        connect(p_spinbox, SIGNAL(valueChanged(double)), p_parent, SLOT(_OnSlotValueChanged(double)));
    }
}

float InspectorFloatSW::GetValue()
{
    return p_spinbox->value();
}

void InspectorFloatSW::OnSpinBoxFocusIn()
{
    _editing = true;
}

void InspectorFloatSW::OnSpinBoxFocusOut()
{
    _editing = false;
    connect(p_spinbox, SIGNAL(valueChanged(double)), p_parent, SLOT(_OnSlotValueChanged(double)));
    p_spinbox->setValue(p_spinbox->value());
    disconnect(p_spinbox, SIGNAL(valueChanged(double)), p_parent, SLOT(_OnSlotValueChanged(double)));
}

QSize InspectorFloatSW::sizeHint() const { return QSize(20, 30); }
