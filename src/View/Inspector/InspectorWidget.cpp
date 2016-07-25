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
    this->m_relatedInspectable = relatedInspectable;

    ConstructFromWidgetInformation( "Inspectable",
                                    relatedInspectable->GetComponentInfo() );

}

InspectorWidget::InspectorWidget(const std::string &title,
                                 InspectorWidgetInfo *widgetInfo,
                                 std::function<void ()> callback)
    : InspectorWidget()
{
    this->m_callback = &callback;
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

    m_titleLabel = new QLabel( QString::fromStdString( title ) );
    QFont font = m_titleLabel->font(); font.setBold(true);
    m_titleLabel->setFont(font); m_titleLabel->show();

    m_titleLayout = new QHBoxLayout();
    m_titleLayout->setContentsMargins(0,0,0,0);
    m_titleLayout->addWidget(m_titleLabel, 10);

    mainLayout->addLayout(m_titleLayout);

    // TODO: Improve THIS :(
    //std::map<std::string, Inspector> infos;
    for (auto it : info->GetSlotInfos())
    {
        InspectorSWInfo *si = it.second;
        InspectorSW *ws = nullptr;

        InspectorVFloatSWInfo *siv = nullptr;
        InspectorEnumSWInfo *sie = nullptr;
        InspectorFileSWInfo *sif = nullptr;
        InspectorStringSWInfo *sis = nullptr;
        InspectorButtonSWInfo *sib = nullptr;

        if ( (siv = dynamic_cast<InspectorVFloatSWInfo*>(si)) !=
                nullptr)
        {
            ws = new InspectorVFloatSW(siv->m_label, siv->m_value, this);
        }
        else if ( (sie = dynamic_cast<InspectorEnumSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorEnumSW(sie->m_label, sie->m_enumValues,
                                     sie->m_selectedValueIndex, this);
        }
        else if ( (sif = dynamic_cast<InspectorFileSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorFileSW(sif->m_label, sif->m_filepath,
                                     sif->m_fileExtension, this);
        }
        else if ( (sis = dynamic_cast<InspectorStringSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorStringSW(sis->m_label, sis->m_value, this,
                                       sis->m_readonly, sis->m_inlined);
        }
        else if ( (sib = dynamic_cast<InspectorButtonSWInfo*>(si)) !=
                 nullptr)
        {
            ws = new InspectorButtonSW(sib->m_label, this,
                                       sib->m_onClickFunction);
        }

        if (ws )
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

    if (autoUpdate)
    {
        m_updateTimer = new QTimer(this); //Every X seconds, update all the slots values
        connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
        m_updateTimer->start(20);
    }
}

InspectorWidget::~InspectorWidget()
{
   delete m_updateTimer;
}

std::vector<float> InspectorWidget::GetSWVectorFloatValue(
        const std::string &slotLabel)
{
    InspectorVFloatSW *w =
            dynamic_cast<InspectorVFloatSW*>(m_labelsToComponentSlots[slotLabel]);
    std::vector<float> r;
    if (w) r = w->GetValue();
    return r;
}

int InspectorWidget::GetSWSelectedEnumIndex(const std::string &slotLabel)
{
    InspectorEnumSW *w =
            dynamic_cast<InspectorEnumSW*>(m_labelsToComponentSlots[slotLabel]);
    if (w) return w->GetValue();
    return 0;
}

std::string InspectorWidget::GetSWFileFilepath(const std::string &slotLabel)
{
    InspectorFileSW *w =
            dynamic_cast<InspectorFileSW*>(m_labelsToComponentSlots[slotLabel]);
    if (w) return w->GetValue();
    return "";
}

void InspectorWidget::OnCustomContextMenuRequested(QPoint point)
{
}

void InspectorWidget::Refresh()
{
    if (m_relatedInspectable)
    {
        Refresh(m_relatedInspectable->GetComponentInfo());
    }
}

void InspectorWidget::Refresh(InspectorWidgetInfo *widgetInfo)
{
    for (auto it : widgetInfo->GetSlotInfos())
    {
        InspectorSWInfo *si = it.second;

        InspectorSW *ws = m_labelsToComponentSlots[si->m_label];
        InspectorVFloatSWInfo* siv = nullptr;
        InspectorEnumSWInfo *sie = nullptr;
        InspectorFileSWInfo *sia = nullptr;
        InspectorStringSWInfo *sis = nullptr;

        if ( (siv = dynamic_cast<InspectorVFloatSWInfo*>(si)) !=
                nullptr)
        {
            InspectorVFloatSW *wv = static_cast<InspectorVFloatSW*>(ws);
            wv->SetValue( siv->m_value );
        }
        else if ( (sie = dynamic_cast<InspectorEnumSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorEnumSW *we = static_cast<InspectorEnumSW*>(ws);
            we->SetValue( sie->m_selectedValueIndex );
        }
        else if ( (sia = dynamic_cast<InspectorFileSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorFileSW *wa = static_cast<InspectorFileSW*>(ws);
            wa->SetValue( sia->m_filepath );
        }
        else if ( (sis = dynamic_cast<InspectorStringSWInfo*>(si)) !=
                 nullptr)
        {
            InspectorStringSW *wss = static_cast<InspectorStringSW*>(ws);
            wss->SetValue( sis->m_value );
        }

        if (ws )
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
    NONULL(m_relatedInspectable);
    m_relatedInspectable->OnSlotValueChanged(this);
}
