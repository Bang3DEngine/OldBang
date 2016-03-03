#include "InspectorPartWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

#include "InspectorPartSlotWidget.h"
#include "InspectorPartEnumSlotWidget.h"
#include "InspectorFloatPartSlotWidget.h"
#include "InspectorVectorFloatPartSlotWidget.h"

InspectorPartWidget::InspectorPartWidget(Part *relatedPart)
    : QWidget()
{
    this->relatedPart = relatedPart;

    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0); mainLayout->setContentsMargins(10,10,10,20);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLabel = new QLabel(QString::fromStdString(relatedPart->GetName()));
    QFont font = titleLabel->font(); font.setBold(true);
    titleLabel->setFont(font); titleLabel->show();

    enabledCheckbox = new QCheckBox();
    enabledCheckbox->setChecked(relatedPart->IsEnabled());
    connect(enabledCheckbox, SIGNAL(clicked(bool)), this, SLOT(OnEnabledCheckboxPressed(bool)));

    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->addWidget(titleLabel, 10);
    titleLayout->addWidget(enabledCheckbox, 1);
    mainLayout->addLayout(titleLayout);

    for(InspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        InspectorPartSlotWidget *ws = nullptr;

        InspectorPartInfoSlotVecFloat* siv;
        InspectorPartInfoSlotEnum* sie;

        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            ws =  new InspectorVectorFloatPartSlotWidget(siv->label, siv->value, this);
        }
        else if( (sie = dynamic_cast<InspectorPartInfoSlotEnum*>(si)) != nullptr)
        {
            ws =  new InspectorPartEnumSlotWidget(sie->label, sie->enumValues, sie->selectedValueIndex, this);
        }

        if(ws != nullptr)
        {
            ws->show();
            mainLayout->addWidget(ws);
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
    InspectorVectorFloatPartSlotWidget *w =
            dynamic_cast<InspectorVectorFloatPartSlotWidget*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

int InspectorPartWidget::GetSelectedEnumSlotIndex(const std::string &slotLabel)
{
    InspectorPartEnumSlotWidget *w =
            dynamic_cast<InspectorPartEnumSlotWidget*>(labelsToPartSlots[slotLabel]);
    int selectedIndex = 0;
    if(w != nullptr) selectedIndex = w->GetValue();
    return selectedIndex;
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

void InspectorPartWidget::OnEnabledCheckboxPressed(bool checked)
{
    if(relatedPart != nullptr)
    {
        relatedPart->SetEnabled(checked);
    }
}

void InspectorPartWidget::_NotifyInspectorSlotChanged(double _)
{
    WindowEventManager::NotifyInspectorSlotChanged(relatedPart, this);
}

void InspectorPartWidget::_NotifyInspectorSlotChanged(QString _)
{
    WindowEventManager::NotifyInspectorSlotChanged(relatedPart, this);
}
