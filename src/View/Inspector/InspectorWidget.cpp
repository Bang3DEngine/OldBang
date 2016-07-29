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
    XMLNode xmlInfo;
    relatedInspectable->OnInspectorXMLNeeded(&xmlInfo);
    ConstructFromWidgetXMLInfo("Inspectable", xmlInfo);
}

InspectorWidget::InspectorWidget(const std::string &title,
                                 XMLNode &widgetXMLInfo,
                                 std::function<void ()> callback)
    : InspectorWidget()
{
    m_callback = &callback;
    ConstructFromWidgetXMLInfo(title, widgetXMLInfo);
}

void InspectorWidget::ConstructFromWidgetXMLInfo(
        const std::string &title,
        XMLNode &xmlInfo,
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
    CreateWidgetSlots(xmlInfo);
    this->adjustSize();
    this->show();

    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(OnCustomContextMenuRequested(QPoint)));

    if (autoUpdate)
    {
        m_updateTimer = new QTimer(this); //Every X seconds, update all the slots values
        connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(RefreshWidgetValues()));
        m_updateTimer->start(20);
    }
}

InspectorWidget::~InspectorWidget()
{
   delete m_updateTimer;
}

void InspectorWidget::OnCustomContextMenuRequested(QPoint point)
{
}

void InspectorWidget::RefreshWidgetValues()
{
    if (m_relatedInspectable)
    {
        XMLNode xmlInfo;
        m_relatedInspectable->OnInspectorXMLNeeded(&xmlInfo);
        RefreshWidgetValues(xmlInfo);
    }
}

void InspectorWidget::RefreshWidgetValues(XMLNode &xmlInfo)
{
    for (auto itAttr : xmlInfo.GetAttributes())
    {
        std::string attrName  = itAttr.first;
        if( m_attrNameToComponentSlots.find(attrName) == m_attrNameToComponentSlots.end())
        {
            continue;
        }

        InspectorSW *ws = m_attrNameToComponentSlots[attrName];

        XMLAttribute attribute = itAttr.second;
        if (attribute.GetType() == XMLAttribute::Type::Float   ||
            attribute.GetType() == XMLAttribute::Type::Vector2 ||
            attribute.GetType() == XMLAttribute::Type::Vector3 ||
            attribute.GetType() == XMLAttribute::Type::Vector4 ||
            attribute.GetType() == XMLAttribute::Type::Quaternion )
        {
            InspectorVFloatSW *wv = static_cast<InspectorVFloatSW*>(ws);
            if (attribute.GetType() == XMLAttribute::Type::Float)
            {
                float v = xmlInfo.GetFloat(attrName);
                wv->SetValue({v});
            }
            else if (attribute.GetType() == XMLAttribute::Type::Vector2)
            {
                glm::vec2 v = xmlInfo.GetVector2(attrName);
                wv->SetValue({v.x, v.y});
            }
            else if (attribute.GetType() == XMLAttribute::Type::Vector3)
            {
                Vector3 v = xmlInfo.GetVector3(attrName);
                wv->SetValue({v.x, v.y, v.z});
            }
            else if (attribute.GetType() == XMLAttribute::Type::Vector4 ||
                     attribute.GetType() == XMLAttribute::Type::Quaternion)
            {
                glm::vec4 v = xmlInfo.GetVector4(attrName);
                wv->SetValue({v.x, v.y, v.z, v.w});
            }
            ws = wv;
        }
        /*
        else if (attribute.GetType() == XMLAttribute::Type::Enum)
        {
            InspectorEnumSW *we = static_cast<InspectorEnumSW*>(ws);
            we->SetValue( xmlInfo.GetInt(attrName) );
            ws = we;
        }
        */
        else if (attribute.GetType() == XMLAttribute::Type::File)
        {
            InspectorFileSW *wa = static_cast<InspectorFileSW*>(ws);
            wa->SetValue( xmlInfo.GetFilepath(attrName) );
            ws = wa;
        }
        else if (attribute.GetType() == XMLAttribute::Type::String)
        {
            InspectorStringSW *wss = static_cast<InspectorStringSW*>(ws);
            wss->SetValue( xmlInfo.GetString(attrName) );
            ws = wss;
        }

        if (ws)
        {
            ws->show();
        }
    }
}

std::vector<float> InspectorWidget::GetSWVectorFloatValue(const std::string &slotLabel)
{
    InspectorVFloatSW *w = dynamic_cast<InspectorVFloatSW*>(m_attrNameToComponentSlots[slotLabel]);
    return w ? w->GetValue() : std::vector<float>();
}

int InspectorWidget::GetSWSelectedEnumIndex(const std::string &slotLabel)
{
    InspectorEnumSW *w = dynamic_cast<InspectorEnumSW*>(m_attrNameToComponentSlots[slotLabel]);
    return w ? w->GetValue() : 0;
}

std::string InspectorWidget::GetSWFileFilepath(const std::string &slotLabel)
{
    InspectorFileSW *w = dynamic_cast<InspectorFileSW*>(m_attrNameToComponentSlots[slotLabel]);
    return w ? w->GetValue() : "";
}

void InspectorWidget::CreateWidgetSlots(XMLNode &xmlInfo)
{
    for (auto itAttr : xmlInfo.GetAttributes())
    {
        std::string attrName  = itAttr.first;
        InspectorSW *ws = nullptr;

        XMLAttribute attribute = itAttr.second;
        if (attribute.GetType() == XMLAttribute::Type::Float)
        {
            ws = new InspectorVFloatSW(attrName, {0}, this, &xmlInfo);
        }
        else if (attribute.GetType() == XMLAttribute::Type::Vector2)
        {
            ws = new InspectorVFloatSW(attrName, {0,0}, this, &xmlInfo);
        }
        else if (attribute.GetType() == XMLAttribute::Type::Vector3)
        {
            ws = new InspectorVFloatSW(attrName, {0,0,0}, this, &xmlInfo);
        }
        else if (attribute.GetType() == XMLAttribute::Type::Vector4 ||
                 attribute.GetType() == XMLAttribute::Type::Quaternion)
        {
            ws = new InspectorVFloatSW(attrName, {0,0,0,0}, this, &xmlInfo);
        }
        else if (attribute.GetType() == XMLAttribute::Type::File)
        {
            ws = new InspectorFileSW(attrName, "", "", this, &xmlInfo);
        }
        else if (attribute.GetType() == XMLAttribute::Type::String)
        {
            ws = new InspectorStringSW(attrName, "", this, &xmlInfo, false, false);
        }

        if (ws)
        {
            ws->show();
            layout()->addWidget(ws);
            m_compSlots.push_back(ws);
            m_attrNameToComponentSlots[attrName] = ws;
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
    XMLNode xmlInfo;
    m_relatedInspectable->OnInspectorXMLNeeded(&xmlInfo);
    m_relatedInspectable->OnInspectorXMLChanged(&xmlInfo);
}
