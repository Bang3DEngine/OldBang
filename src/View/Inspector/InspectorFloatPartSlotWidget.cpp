#include "InspectorFloatPartSlotWidget.h"

#include "InspectorPartWidget.h"

InspectorPartSlotWidget<float>::InspectorPartSlotWidget(InspectorPartWidget *parent,
                                                        const std::string &labelString,
                                                        float value) :
                             InspectorPartSlotWidget(parent, labelString, value)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0); layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    if(labelString != "")
    {
        QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
        textLabel->setContentsMargins(0,0,0,0);
        textLabel->show();
        layout->addWidget(textLabel);
    }

    spinbox = new FloatPartSlotSpinBox();
    spinbox->setValue(value);
    spinbox->setAlignment(Qt::AlignHCenter);
    spinbox->setMinimum(-999999999.9);
    spinbox->setMaximum(999999999.9);
    spinbox->setAccelerated(true);
    spinbox->setMinimumWidth(50);
    spinbox->setContentsMargins(0,0,0,0);
    spinbox->show();

    layout->addWidget(spinbox);

    this->setContentsMargins(0,0,0,0);
    this->show();

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorPartSlotWidget<float>::SetValue(float f)
{
    if(!editing)
    {
        disconnect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
        spinbox->setValue(f);
        connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
    }
}

float InspectorPartSlotWidget<float>::GetValue()
{
    return spinbox->value();
}

void InspectorPartSlotWidget<float>::OnSpinBoxFocusIn()
{
    editing = true;
}

void InspectorPartSlotWidget<float>::OnSpinBoxFocusOut()
{
    editing = false;
    connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
    spinbox->setValue(spinbox->value());
    disconnect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
}

QSize InspectorPartSlotWidget<float>::sizeHint() const { return QSize(30, 30); }
