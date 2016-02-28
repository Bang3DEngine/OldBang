#include "InspectorPartWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

#include "InspectorPartSlotWidget.h"
#include "InspectorFloatPartSlotWidget.h"
#include "InspectorVectorFloatPartSlotWidget.h"

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
        InspectorPartSlotWidget *ws = nullptr;

        InspectorPartInfoSlotVecFloat* siv;  //If the infoSlot is of type VecFloat
        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            ws =  new InspectorVectorFloatPartSlotWidget(siv->value, siv->label, this);
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
    InspectorVectorFloatPartSlotWidget *w = dynamic_cast<InspectorVectorFloatPartSlotWidget*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

void InspectorPartWidget::UpdateSlotsValues()
{
    for(InspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        InspectorPartSlotWidget *ws = labelsToPartSlots[si->label];
        InspectorPartInfoSlotVecFloat* siv;
        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            InspectorVectorFloatPartSlotWidget *wv = static_cast<InspectorVectorFloatPartSlotWidget*>(ws);
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
