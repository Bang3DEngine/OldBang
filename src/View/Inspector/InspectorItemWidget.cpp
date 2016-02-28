#include "InspectorItemWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

InspectorPartWidget::InspectorPartWidget(Part *relatedPart)
    : QWidget()
{
    this->relatedPart = relatedPart;

    setLayout(new QVBoxLayout());
    layout()->setSpacing(0); layout()->setContentsMargins(0,0,0,0);

    titleLabel = new QLabel(QString::fromStdString(relatedPart->GetName()));
    QFont font = titleLabel->font(); font.setBold(true);
    titleLabel->setFont(font); titleLabel->show();
    layout()->addWidget(titleLabel);

    for(InspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        SlotWidget *ws = nullptr;

        InspectorPartInfoSlotVecFloat* siv;  //If the infoSlot is of type VecFloat
        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            ws =  new VectorFloatSlotWidget(siv->value, siv->label, this);
        }

        if(ws != nullptr)
        {
            ws->show();
            layout()->addWidget(ws);
            partSlots.push_back(ws);
            labelsToPartSlots[si->label] = ws;
        }
    }

    this->show();

    updateTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(UpdateSlotsValues()));
    updateTimer->start(20);
}

InspectorPartWidget::~InspectorPartWidget()
{
   delete updateTimer;
}

std::vector<float> InspectorPartWidget::GetVectorFloatSlotValue(const std::string &slotLabel)
{
    VectorFloatSlotWidget *w = dynamic_cast<VectorFloatSlotWidget*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

const std::string InspectorPartWidget::FloatToString(float f)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << f;
    return ss.str();
}

InspectorPartWidget::FloatSlotWidget::FloatSlotWidget(float initialValue,
                                                          const std::string &labelString,
                                                          InspectorPartWidget *parent) : SlotWidget(parent)
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

    spinbox = new QDoubleSpinBox();
    spinbox->setValue(initialValue);
    spinbox->setAlignment(Qt::AlignHCenter);
    spinbox->setMinimum(-999999999.9);
    spinbox->setMaximum(999999999.9);
    spinbox->setAccelerated(true);
    spinbox->setMinimumWidth(50);
    spinbox->setContentsMargins(0,0,0,0);
    spinbox->show();
    connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));

    layout->addWidget(spinbox);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorPartWidget::FloatSlotWidget::SetValue(float f)
{
    disconnect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
    spinbox->setValue( double(f) );
    connect(spinbox, SIGNAL(valueChanged(double)), parent, SLOT(_NotifyInspectorSlotChanged(double)));
}

float InspectorPartWidget::FloatSlotWidget::GetValue()
{
    return float(spinbox->value());
}

InspectorPartWidget::VectorFloatSlotWidget::VectorFloatSlotWidget(std::vector<float> initialValues,
                                                            const std::string &labelString,
                                                            InspectorPartWidget *parent) : SlotWidget(parent)
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
        FloatSlotWidget *s = new FloatSlotWidget(initialValues[i], "", parent);
        floatSlots.push_back(s);

        s->setContentsMargins(0,0,0,0); s->show();
        hLayout->addWidget(s);
    }

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorPartWidget::VectorFloatSlotWidget::SetValue(const std::vector<float> &v)
{
    for(unsigned int i = 0; i < floatSlots.size(); ++i)
    {
        floatSlots[i]->SetValue(v[i]);
    }
}

std::vector<float>  InspectorPartWidget::VectorFloatSlotWidget::GetValue()
{
    std::vector<float> result;
    for(unsigned int i = 0; i < floatSlots.size(); ++i)
    {
        float f = floatSlots[i]->GetValue();
        result.push_back(f);
    }
    return result;
}

void InspectorPartWidget::UpdateSlotsValues()
{
    for(InspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        SlotWidget *ws = labelsToPartSlots[si->label];
        InspectorPartInfoSlotVecFloat* siv;
        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            VectorFloatSlotWidget *wv = static_cast<VectorFloatSlotWidget*>(ws);
            wv->SetValue( siv->value );
        }

        if(ws != nullptr)
        {
            ws->show();
            layout()->addWidget(ws);
            partSlots.push_back(ws);
            labelsToPartSlots[si->label] = ws;
        }
    }
}

void InspectorPartWidget::_NotifyInspectorSlotChanged(double newValue)
{
    WindowEventManager::NotifyInspectorSlotChanged(relatedPart, this);
}


void InspectorPartWidget::SlotWidget::focusInEvent(QFocusEvent *event)
{
    Logger_Log("Focus in");
}

void InspectorPartWidget::SlotWidget::focusOutEvent(QFocusEvent *event)
{
    Logger_Log("Focus out");
}
