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

    this->show();

    UpdateSlotsValues();
    updateTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(UpdateSlotsValues()));
    updateTimer->start(20);
}

InspectorPartWidget::~InspectorPartWidget()
{
   delete updateTimer;
}

void InspectorPartWidget::UpdateSlotsValues()
{
    for(InspectorPartSlotInfoBase *si : relatedPart->GetInfo()->slotInfos)
    {
        InspectorPartSlotWidgetBase *ws = nullptr;

        InspectorPartSlotInfo< std::vector<float> >* siv;
        InspectorPartSlotInfo< std::vector<std::string> >* sie;

        if( (siv = dynamic_cast<InspectorPartSlotInfo< std::vector<float> > *>(si)) != nullptr)
        {
            ws = new InspectorPartSlotWidget< std::vector<float> >(this, siv->GetLabel(), siv->GetValue());
        }
        else if( (sie = dynamic_cast<InspectorPartSlotInfo< std::vector<std::string> > *>(si)) != nullptr)
        {
            ws = new InspectorPartSlotWidget< std::vector<std::string> >(this, sie->GetLabel(),
                                                                               sie->GetValue(),
                                                                               sie->GetSelectedIndex() );
        }

        if(ws != nullptr)
        {
            ws->show();
            layout()->addWidget(ws);
            labelsToPartSlots[slotInfo->GetLabel()] = ws;
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
