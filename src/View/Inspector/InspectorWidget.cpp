#include "InspectorWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

#include "InspectorPartSW.h"
#include "InspectorPartEnumSW.h"
#include "InspectorPartFileSW.h"
#include "InspectorPartFloatSW.h"
#include "InspectorPartVFloatSW.h"

#include "WindowMain.h"
#include "Entity.h"

InspectorWidget::InspectorWidget(Part *relatedInspectable)
    : QWidget()
{
    this->relatedInspectable = relatedInspectable;

    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0); mainLayout->setContentsMargins(10,10,10,20);

    titleLayout = new QHBoxLayout();
    titleLabel = new QLabel(QString::fromStdString(relatedInspectable->GetName()));
    QFont font = titleLabel->font(); font.setBold(true);
    titleLabel->setFont(font); titleLabel->show();

    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->addWidget(titleLabel, 10);
    mainLayout->addLayout(titleLayout);

    for(InspectorPartSlotInfo *si : relatedInspectable->GetPartInfo()->slotInfos)
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
            ws = new InspectorPartFileSW(sia->label, sia->filepath, sia->fileExtension, this);
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

    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnCustomContextMenuRequested(QPoint)));

    updateTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(UpdateSlotsValues()));
    updateTimer->start(20);
}

InspectorWidget::~InspectorWidget()
{
   delete updateTimer;
}

std::vector<float> InspectorWidget::GetSWVectorFloatValue(const std::string &slotLabel)
{
    InspectorPartVFloatSW *w =
            dynamic_cast<InspectorPartVFloatSW*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

int InspectorWidget::GetSWSelectedEnumIndex(const std::string &slotLabel)
{
    InspectorPartEnumSW *w =
            dynamic_cast<InspectorPartEnumSW*>(labelsToPartSlots[slotLabel]);
    if(w != nullptr) return w->GetValue();
    return 0;
}

std::string InspectorWidget::GetSWFileFilepath(const std::string &slotLabel)
{
    InspectorPartFileSW *w =
            dynamic_cast<InspectorPartFileSW*>(labelsToPartSlots[slotLabel]);
    if(w != nullptr) return w->GetValue();
    return "";
}

void InspectorWidget::OnCustomContextMenuRequested(QPoint point)
{
}

void InspectorWidget::UpdateSlotsValues()
{
    for(InspectorPartSlotInfo *si : relatedInspectable->GetPartInfo()->slotInfos)
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
            InspectorPartFileSW *wa = static_cast<InspectorPartFileSW*>(ws);
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

void InspectorWidget::_NotifyInspectorSlotChanged(double _)
{
    relatedInspectable->OnSlotValueChanged(this);
}

void InspectorWidget::_NotifyInspectorSlotChanged(QString _)
{
    relatedInspectable->OnSlotValueChanged(this);
}

void InspectorWidget::_NotifyInspectorSlotChanged()
{
    relatedInspectable->OnSlotValueChanged(this);
}
