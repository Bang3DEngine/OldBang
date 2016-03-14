#include "InspectorPartWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

#include "InspectorPartSW.h"
#include "InspectorPartEnumSW.h"
#include "InspectorPartAssetSW.h"
#include "InspectorPartFloatSW.h"
#include "InspectorPartVFloatSW.h"

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
        InspectorPartSW *ws = nullptr;

        InspectorPartInfoSlotVecFloat *siv;
        InspectorPartInfoSlotEnum *sie;
        InspectorPartInfoSlotAsset *sia;

        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            ws = new InspectorPartVFloatSW(siv->label, siv->value, this);
        }
        else if( (sie = dynamic_cast<InspectorPartInfoSlotEnum*>(si)) != nullptr)
        {
            ws = new InspectorPartEnumSW(sie->label, sie->enumValues, sie->selectedValueIndex, this);
        }
        else if( (sia = dynamic_cast<InspectorPartInfoSlotAsset*>(si)) != nullptr)
        {
            ws = new InspectorPartAssetSW(sia->label, sia->filepath, sia->fileExtension, this);
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
    InspectorPartVFloatSW *w =
            dynamic_cast<InspectorPartVFloatSW*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

int InspectorPartWidget::GetSelectedEnumSlotIndex(const std::string &slotLabel)
{
    InspectorPartEnumSW *w =
            dynamic_cast<InspectorPartEnumSW*>(labelsToPartSlots[slotLabel]);
    int selectedIndex = 0;
    if(w != nullptr) selectedIndex = w->GetValue();
    return selectedIndex;
}

void InspectorPartWidget::UpdateSlotsValues()
{
    for(InspectorPartSlotInfo *si : relatedPart->GetInfo()->slotInfos)
    {
        InspectorPartSW *ws = labelsToPartSlots[si->label];
        InspectorPartInfoSlotVecFloat* siv;
        InspectorPartInfoSlotEnum *sie;
        InspectorPartInfoSlotAsset *sia;

        if( (siv = dynamic_cast<InspectorPartInfoSlotVecFloat*>(si)) != nullptr)
        {
            InspectorPartVFloatSW *wv = static_cast<InspectorPartVFloatSW*>(ws);
            wv->SetValue( siv->value );
        }
        else if( (sie = dynamic_cast<InspectorPartInfoSlotEnum*>(si)) != nullptr)
        {
            InspectorPartEnumSW *we = static_cast<InspectorPartEnumSW*>(ws);
            we->SetValue( sie->selectedValueIndex );
        }
        else if( (sia = dynamic_cast<InspectorPartInfoSlotAsset*>(si)) != nullptr)
        {
            InspectorPartAssetSW *wa = static_cast<InspectorPartAssetSW*>(ws);
            wa->SetValue( sia->filepath );
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
