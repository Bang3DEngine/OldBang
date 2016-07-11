#include "InspectorWidget.h"
#include "WindowEventManager.h"
#include "Component.h"

#include "InspectorSW.h"
#include "InspectorEnumSW.h"
#include "InspectorFileSW.h"
#include "InspectorFloatSW.h"
#include "InspectorStringSW.h"
#include "InspectorVFloatSW.h"
#include "InspectorButtonSW.h"

#include "WindowMain.h"
#include "GameObject.h"

InspectorWidget::InspectorWidget() : QWidget()
{
}

InspectorWidget::InspectorWidget(IInspectable *relatedInspectable)
    : InspectorWidget()
{
    this->p_relatedInspectable = relatedInspectable;

    ConstructFromWidgetInformation( "Inspectable",
                                    relatedInspectable->GetComponentInfo() );

}

InspectorWidget::InspectorWidget(const std::string &title,
                                 InspectorWidgetInfo *widgetInfo,
                                 std::function<void ()> callback)
    : InspectorWidget()
{
    this->p_callback = &callback;
    this->ConstructFromWidgetInformation(title, widgetInfo);
}

void InspectorWidget::ConstructFromWidgetInformation(
        const std::string &title,
        const InspectorWidgetInfo *info,
        bool autoUpdate
        )
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(5,5,5,10);

    p_titleLabel = new QLabel( QString::fromStdString( title ) );
    QFont font = p_titleLabel->font(); font.setBold(true);
    p_titleLabel->setFont(font); p_titleLabel->show();

    p_titleLayout = new QHBoxLayout();
    p_titleLayout->setContentsMargins(0,0,0,0);
    p_titleLayout->addWidget(p_titleLabel, 10);

    mainLayout->addLayout(p_titleLayout);

    // TODO: Improve THIS :(
    for(InspectorSWInfo *si : info->GetSlotInfos())
    {
        InspectorSW *ws = nullptr;

        InspectorVFloatSWInfo *siv = nullptr;
        InspectorEnumSWInfo *sie = nullptr;
        InspectorFileSWInfo *sif = nullptr;
        InspectorStringSWInfo *sis = nullptr;
        InspectorButtonSWInfo *sib = nullptr;

        if( (siv = dynamic_cast<InspectorVFloatSWInfo*>(si)) !=
                nullptr)
        {
            ws = new InspectorVFloatSW(siv->m_label, siv->m_value, this);
        }
        else if( (sie = dynamic_cast<InspectorEnumSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorEnumSW(sie->m_label, sie->enumValues,
                                     sie->selectedValueIndex, this);
        }
        else if( (sif = dynamic_cast<InspectorFileSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorFileSW(sif->m_label, sif->filepath,
                                     sif->fileExtension, this);
        }
        else if( (sis = dynamic_cast<InspectorStringSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorStringSW(sis->m_label, sis->value, this,
                                       sis->readonly, sis->inlined);
        }
        else if( (sib = dynamic_cast<InspectorButtonSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorButtonSW(sib->m_label, this,
                                       sib->onClickFunction);
        }

        if(ws )
        {
            ws->show();
            mainLayout->addWidget(ws);
            m_compSlots.push_back(ws);
            m_labelsToComponentSlots[si->m_label] = ws;
        }
    }

    this->show();

    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnCustomContextMenuRequested(QPoint)));

    if(autoUpdate)
    {
        p_updateTimer = new QTimer(this); //Every X seconds, update all the slots values
        connect(p_updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
        p_updateTimer->start(20);
    }
}

InspectorWidget::~InspectorWidget()
{
   delete p_updateTimer;
}

std::vector<float> InspectorWidget::GetSWVectorFloatValue(
        const std::string &slotLabel)
{
    InspectorVFloatSW *w =
            dynamic_cast<InspectorVFloatSW*>(m_labelsToComponentSlots[slotLabel]);
    std::vector<float> r;
    if(w ) r = w->GetValue();
    return r;
}

int InspectorWidget::GetSWSelectedEnumIndex(const std::string &slotLabel)
{
    InspectorEnumSW *w =
            dynamic_cast<InspectorEnumSW*>(m_labelsToComponentSlots[slotLabel]);
    if(w ) return w->GetValue();
    return 0;
}

std::string InspectorWidget::GetSWFileFilepath(const std::string &slotLabel)
{
    InspectorFileSW *w =
            dynamic_cast<InspectorFileSW*>(m_labelsToComponentSlots[slotLabel]);
    if(w ) return w->GetValue();
    return "";
}

void InspectorWidget::OnCustomContextMenuRequested(QPoint point)
{
}

void InspectorWidget::Refresh()
{
    if(p_relatedInspectable )
    {
        Refresh(p_relatedInspectable->GetComponentInfo());
    }
}

void InspectorWidget::Refresh(InspectorWidgetInfo *widgetInfo)
{
    for(InspectorSWInfo *si : widgetInfo->GetSlotInfos())
    {
        InspectorSW *ws = m_labelsToComponentSlots[si->m_label];
        InspectorVFloatSWInfo* siv = nullptr;
        InspectorEnumSWInfo *sie = nullptr;
        InspectorFileSWInfo *sia = nullptr;
        InspectorStringSWInfo *sis = nullptr;

        if( (siv = dynamic_cast<InspectorVFloatSWInfo*>(si)) !=
                nullptr)
        {
            InspectorVFloatSW *wv = static_cast<InspectorVFloatSW*>(ws);
            wv->SetValue( siv->m_value );
        }
        else if( (sie = dynamic_cast<InspectorEnumSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorEnumSW *we = static_cast<InspectorEnumSW*>(ws);
            we->SetValue( sie->selectedValueIndex );
        }
        else if( (sia = dynamic_cast<InspectorFileSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorFileSW *wa = static_cast<InspectorFileSW*>(ws);
            wa->SetValue( sia->filepath );
        }
        else if( (sis = dynamic_cast<InspectorStringSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorStringSW *wss = static_cast<InspectorStringSW*>(ws);
            wss->SetValue( sis->value );
        }

        if(ws )
        {
            ws->show();
            layout()->addWidget(ws);
            m_compSlots.push_back(ws);
            m_labelsToComponentSlots[si->m_label] = ws;
        }
    }
}

void InspectorWidget::_OnSlotValueChanged(double _)
{
    _OnSlotValueChanged();
}

void InspectorWidget::_OnSlotValueChanged(QString _)
{
    _OnSlotValueChanged();
}

void InspectorWidget::_OnSlotValueChanged()
{
    NONULL(p_relatedInspectable);
    p_relatedInspectable->OnSlotValueChanged(this);
}
