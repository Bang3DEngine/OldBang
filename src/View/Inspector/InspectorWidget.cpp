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
    m_relatedInspectable = relatedInspectable;

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
        m_updateTimer->start(100);
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
        if( m_attrNameToAttrWidget.find(attrName) == m_attrNameToAttrWidget.end())
        {
            continue;
        }

        InspectorSW *ws = m_attrNameToAttrWidget[attrName];

        XMLAttribute attribute = itAttr.second;
        XMLAttribute::Type attrType = attribute.GetType();
        if (attrType == XMLAttribute::Type::TFloat   ||
            attrType == XMLAttribute::Type::TVector2 ||
            attrType == XMLAttribute::Type::TVector3 ||
            attrType == XMLAttribute::Type::TVector4 ||
            attrType == XMLAttribute::Type::TQuaternion )
        {
            InspectorVFloatSW *wv = static_cast<InspectorVFloatSW*>(ws);
            if (attrType == XMLAttribute::Type::TFloat)
            {
                float v = xmlInfo.GetFloat(attrName);
                wv->SetValue({v});
            }
            else if (attrType == XMLAttribute::Type::TVector2)
            {
                glm::vec2 v = xmlInfo.GetVector2(attrName);
                wv->SetValue({v.x, v.y});
            }
            else if (attrType == XMLAttribute::Type::TVector3)
            {
                Vector3 v = xmlInfo.GetVector3(attrName);
                wv->SetValue({v.x, v.y, v.z});
            }
            else if (attrType == XMLAttribute::Type::TVector4 ||
                     attrType == XMLAttribute::Type::TQuaternion)
            {
                glm::vec4 v = xmlInfo.GetVector4(attrName);
                wv->SetValue({v.x, v.y, v.z, v.w});
            }
            ws = wv;
        }
        /*
        else if (attrType == XMLAttribute::Type::Enum)
        {
            InspectorEnumSW *we = static_cast<InspectorEnumSW*>(ws);
            we->SetValue( xmlInfo.GetInt(attrName) );
            ws = we;
        }
        */
        else if (attrType == XMLAttribute::Type::TFile)
        {
            InspectorFileSW *wa = static_cast<InspectorFileSW*>(ws);
            wa->SetValue( xmlInfo.GetFilepath(attrName) );
            ws = wa;
        }
        else if (attrType == XMLAttribute::Type::TString)
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
    InspectorVFloatSW *w = dynamic_cast<InspectorVFloatSW*>(m_attrNameToAttrWidget[slotLabel]);
    return w ? w->GetValue() : std::vector<float>();
}

int InspectorWidget::GetSWSelectedEnumIndex(const std::string &slotLabel)
{
    InspectorEnumSW *w = dynamic_cast<InspectorEnumSW*>(m_attrNameToAttrWidget[slotLabel]);
    return w ? w->GetValue() : 0;
}

std::string InspectorWidget::GetSWFileFilepath(const std::string &slotLabel)
{
    InspectorFileSW *w = dynamic_cast<InspectorFileSW*>(m_attrNameToAttrWidget[slotLabel]);
    return w ? w->GetValue() : "";
}

void InspectorWidget::CreateWidgetSlots(XMLNode &xmlInfo)
{
    for (auto itAttr : xmlInfo.GetAttributes())
    {
        XMLAttribute attribute = itAttr.second;
        m_attributes.push_back(attribute);
        if (!attribute.HasProperty(XMLProperty::Hidden))
        {
            std::string attrName  = attribute.GetName();
            XMLAttribute::Type attrType = attribute.GetType();
            InspectorSW *ws = nullptr;

            if (attrType == XMLAttribute::Type::TFloat)
            {
                ws = new InspectorVFloatSW(attrName, {0}, this, &xmlInfo);
            }
            else if (attrType == XMLAttribute::Type::TVector2)
            {
                ws = new InspectorVFloatSW(attrName, {0,0}, this, &xmlInfo);
            }
            else if (attrType == XMLAttribute::Type::TVector3)
            {
                ws = new InspectorVFloatSW(attrName, {0,0,0}, this, &xmlInfo);
            }
            else if (attrType == XMLAttribute::Type::TVector4 ||
                     attrType == XMLAttribute::Type::TQuaternion)
            {
                ws = new InspectorVFloatSW(attrName, {0,0,0,0}, this, &xmlInfo);
            }
            else if (attrType == XMLAttribute::Type::TFile)
            {
                std::string fileExtension =
                        attribute.GetPropertyValue(XMLProperty::FileExtension.GetName());
                ws = new InspectorFileSW(attrName, "", fileExtension, this, &xmlInfo);
            }
            else if (attrType == XMLAttribute::Type::TString)
            {
                ws = new InspectorStringSW(attrName, "", this, &xmlInfo, false, false);
            }

            if (ws)
            {
                ws->show();
                layout()->addWidget(ws);
                m_attributeWidgets.push_back(ws);
                m_attrNameToAttrWidget[attrName] = ws;
                m_attrWidgetToAttribute[ws] = attribute;
            }
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

    // Create the xmlInfo with all the updated values!
    XMLNode xmlInfo;
    //Logger_Log(m_componentSlotsToAttribute);
    for (XMLAttribute attribute : m_attributes)
    {
        std::string attrName = attribute.GetName();
        if (m_attrNameToAttrWidget.find(attrName) != m_attrNameToAttrWidget.end())
        {
            InspectorSW *ws = m_attrNameToAttrWidget[attrName];
            XMLAttribute::Type attrType = attribute.GetType();

            if (attrType == XMLAttribute::Type::TFloat   ||
                attrType == XMLAttribute::Type::TVector2 ||
                attrType == XMLAttribute::Type::TVector3 ||
                attrType == XMLAttribute::Type::TVector4 ||
                attrType == XMLAttribute::Type::TQuaternion )
            {
                InspectorVFloatSW *wv = static_cast<InspectorVFloatSW*>(ws);
                std::vector<float> v = wv->GetValue();
                if (attrType == XMLAttribute::Type::TFloat)
                {
                    attribute.SetFloat(v[0],
                                       attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::TVector2)
                {
                    attribute.SetVector2(glm::vec2(v[0], v[1]),
                                         attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::TVector3)
                {
                    attribute.SetVector3(Vector3(v[0], v[1], v[2]),
                                         attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::TVector4 ||
                         attrType == XMLAttribute::Type::TQuaternion)
                {
                    attribute.SetVector4(glm::vec4(v[0], v[1], v[2], v[3]),
                                         attribute.GetProperties());
                }
            }
            /*
            else if (attrType == XMLAttribute::Type::Enum)
            {
                InspectorEnumSW *we = static_cast<InspectorEnumSW*>(ws);
            }
            */
            else if (attrType == XMLAttribute::Type::TFile)
            {
                InspectorFileSW *wf = static_cast<InspectorFileSW*>(ws);
                attribute.SetFilepath(wf->GetValue(),
                                      attribute.GetPropertyValue(XMLProperty::FileExtension.GetName()),
                                      attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::TString)
            {
                InspectorStringSW *wss = static_cast<InspectorStringSW*>(ws);
                attribute.SetString(wss->GetValue(), attribute.GetProperties());
            }
        }

        xmlInfo.SetAttribute(attribute);
    }

    m_relatedInspectable->OnInspectorXMLChanged(&xmlInfo);
}
