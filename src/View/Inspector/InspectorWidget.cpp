#include "InspectorWidget.h"
#include "WindowEventManager.h"
#include "Part.h"

#include "InspectorSW.h"
#include "InspectorEnumSW.h"
#include "InspectorFileSW.h"
#include "InspectorFloatSW.h"
#include "InspectorVFloatSW.h"

#include "WindowMain.h"
#include "Entity.h"

InspectorWidget::InspectorWidget(IInspectable *relatedInspectable)
    : QWidget()
{
    this->relatedInspectable = relatedInspectable;

    ConstructFromWidgetInformation( "Inspectable",
                                    relatedInspectable->GetPartInfo() );

    updateTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    updateTimer->start(20);
}

InspectorWidget::InspectorWidget(const std::string &title,
                                 InspectorWidgetInfo *widgetInfo,
                                 std::function<void ()> callback)
{
    this->callback = &callback;
    this->ConstructFromWidgetInformation(title, widgetInfo);
}

void InspectorWidget::ConstructFromWidgetInformation(
        const std::string &title,
        const InspectorWidgetInfo *info
        )
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0); mainLayout->setContentsMargins(10,10,10,20);

    titleLayout = new QHBoxLayout();
    titleLabel = new QLabel(
                QString::fromStdString( title )
                );
    QFont font = titleLabel->font(); font.setBold(true);
    titleLabel->setFont(font); titleLabel->show();

    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->addWidget(titleLabel, 10);
    mainLayout->addLayout(titleLayout);

    for(InspectorSWInfo *si : info->GetSlotInfos())
    {
        InspectorSW *ws = nullptr;

        InspectorVFloatSWInfo *siv;
        InspectorEnumSWInfo *sie;
        InspectorAssetSWInfo *sia;

        if( (siv = dynamic_cast<InspectorVFloatSWInfo*>(si)) !=
                nullptr)
        {
            ws = new InspectorVFloatSW(siv->label, siv->value, this);
        }
        else if( (sie = dynamic_cast<InspectorEnumSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorEnumSW(sie->label, sie->enumValues,
                                     sie->selectedValueIndex, this);
        }
        else if( (sia = dynamic_cast<InspectorAssetSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorFileSW(sia->label, sia->filepath,
                                     sia->fileExtension, this);
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

}

InspectorWidget::~InspectorWidget()
{
   delete updateTimer;
}

std::vector<float> InspectorWidget::GetSWVectorFloatValue(
        const std::string &slotLabel)
{
    InspectorVFloatSW *w =
            dynamic_cast<InspectorVFloatSW*>(labelsToPartSlots[slotLabel]);
    std::vector<float> r;
    if(w != nullptr) r = w->GetValue();
    return r;
}

int InspectorWidget::GetSWSelectedEnumIndex(const std::string &slotLabel)
{
    InspectorEnumSW *w =
            dynamic_cast<InspectorEnumSW*>(labelsToPartSlots[slotLabel]);
    if(w != nullptr) return w->GetValue();
    return 0;
}

std::string InspectorWidget::GetSWFileFilepath(const std::string &slotLabel)
{
    InspectorFileSW *w =
            dynamic_cast<InspectorFileSW*>(labelsToPartSlots[slotLabel]);
    if(w != nullptr) return w->GetValue();
    return "";
}

void InspectorWidget::OnCustomContextMenuRequested(QPoint point)
{
}

void InspectorWidget::Refresh()
{
    if(relatedInspectable != nullptr)
    {
        Refresh(relatedInspectable->GetPartInfo());
    }
}

void InspectorWidget::Refresh(InspectorWidgetInfo *widgetInfo)
{
    for(InspectorSWInfo *si : widgetInfo->GetSlotInfos())
    {
        InspectorSW *ws = labelsToPartSlots[si->label];
        InspectorVFloatSWInfo* siv;
        InspectorEnumSWInfo *sie;
        InspectorAssetSWInfo *sia;

        if( (siv = dynamic_cast<InspectorVFloatSWInfo*>(si)) !=
                nullptr)
        {
            InspectorVFloatSW *wv = static_cast<InspectorVFloatSW*>(ws);
            wv->SetValue( siv->value );
        }
        else if( (sie = dynamic_cast<InspectorEnumSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorEnumSW *we = static_cast<InspectorEnumSW*>(ws);
            we->SetValue( sie->selectedValueIndex );
        }
        else if( (sia = dynamic_cast<InspectorAssetSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorFileSW *wa = static_cast<InspectorFileSW*>(ws);
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

void InspectorWidget::_OnSlotValueChanged(double _)
{
    relatedInspectable->OnSlotValueChanged(this);
}

void InspectorWidget::_OnSlotValueChanged(QString _)
{
    relatedInspectable->OnSlotValueChanged(this);
}

void InspectorWidget::_OnSlotValueChanged()
{
    relatedInspectable->OnSlotValueChanged(this);
}
