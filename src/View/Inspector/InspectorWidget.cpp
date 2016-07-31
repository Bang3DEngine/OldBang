#include "InspectorWidget.h"
#include "WindowEventManager.h"
#include "Component.h"

#include "AttributeWidget.h"
#include "AttrWidgetEnum.h"
#include "AttrWidgetFile.h"
#include "AttrWidgetFloat.h"
#include "AttrWidgetString.h"
#include "AttrWidgetVectorFloat.h"
#include "AttrWidgetButton.h"

#include "WindowMain.h"
#include "GameObject.h"

InspectorWidget::InspectorWidget() : QWidget() {}

InspectorWidget::InspectorWidget(const std::string &title,
                                 IInspectable *relatedInspectable) : InspectorWidget()
{
    m_relatedInspectable = relatedInspectable;

    XMLNode xmlInfo = GetInspectableXMLInfo();
    ConstructFromWidgetXMLInfo(title, xmlInfo);
}

void InspectorWidget::ConstructFromWidgetXMLInfo(
        const std::string &title, XMLNode &xmlInfo, bool autoUpdate)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    m_titleLayout = new QHBoxLayout();

    std::string fTitle = StringUtils::FormatInspectorLabel(title);
    m_titleLabel = new QLabel( QString(fTitle.c_str()) );
    QFont font = m_titleLabel->font();
    font.setBold(true);
    m_titleLabel->setFont(font);

    m_titleLayout->addWidget(m_titleLabel, 10);

    mainLayout->addLayout(m_titleLayout);
    CreateWidgetSlots(xmlInfo);
    RefreshWidgetValues(); // Initial catch of values
    m_created = true;
    show();

    //

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

XMLNode InspectorWidget::GetInspectableXMLInfo() const
{
    XMLNode xmlInfo;
    m_relatedInspectable->OnInspectorXMLNeeded(&xmlInfo);
    return xmlInfo;
}

XMLNode InspectorWidget::GetWidgetXMLInfo() const
{
    // Gather all attributes
    XMLNode xmlInfo;
    xmlInfo.SetTagName(m_tagName);
    for (XMLAttribute attribute : m_attributes)
    {
        std::string attrName = attribute.GetName();
        XMLAttribute::Type attrType = attribute.GetType();

        if (m_attrNameToAttrWidget.find(attrName) != m_attrNameToAttrWidget.end())
        {
            AttributeWidget *aw = m_attrNameToAttrWidget[attrName];
            if (attribute.HasVectoredType())
            {
                AttrWidgetVectorFloat *awv = static_cast<AttrWidgetVectorFloat*>(aw);
                std::vector<float> v = awv->GetValue();
                if (attrType == XMLAttribute::Type::TFloat)
                {
                    attribute.SetFloat(v[0], attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::TVector2)
                {
                    attribute.SetVector2(glm::vec2(v[0], v[1]), attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::TVector3)
                {
                    attribute.SetVector3(Vector3(v[0], v[1], v[2]), attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::TVector4 ||
                         attrType == XMLAttribute::Type::TQuaternion)
                {
                    attribute.SetVector4(glm::vec4(v[0], v[1], v[2], v[3]), attribute.GetProperties());
                }
            }
            else if (attrType == XMLAttribute::Type::TFile)
            {
                AttrWidgetFile *awf = static_cast<AttrWidgetFile*>(aw);
                attribute.SetFilepath(awf->GetValue(),
                                      attribute.GetPropertyValue(XMLProperty::FileExtension.GetName()),
                                      attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::TString)
            {
                AttrWidgetString *aws = static_cast<AttrWidgetString*>(aw);
                attribute.SetString(aws->GetValue(), attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::TEnum)
            {
                AttrWidgetEnum *awe = static_cast<AttrWidgetEnum*>(aw);
                attribute.SetEnum(attribute.GetEnumNames(), awe->GetValue(), // selected index
                                  attribute.GetProperties());
            }
        }
        xmlInfo.SetAttribute(attribute);
    }

    return xmlInfo;
}

void InspectorWidget::OnCustomContextMenuRequested(QPoint point)
{
}

void InspectorWidget::RefreshWidgetValues()
{
    XMLNode xmlInfo = GetInspectableXMLInfo();
    xmlInfo.SetTagName(m_tagName);
    for (auto itAttr : xmlInfo.GetAttributes())
    {
        XMLAttribute attribute = itAttr.second;
        std::string attrName  = attribute.GetName();
        XMLAttribute::Type attrType = attribute.GetType();

        if( m_attrNameToAttrWidget.find(attrName) != m_attrNameToAttrWidget.end())
        {
            AttributeWidget *ws = m_attrNameToAttrWidget[attrName];
            if (attribute.HasVectoredType())
            {
                AttrWidgetVectorFloat *wv = static_cast<AttrWidgetVectorFloat*>(ws);
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
            else if (attrType == XMLAttribute::Type::TFile)
            {
                AttrWidgetFile *wf = static_cast<AttrWidgetFile*>(ws);
                wf->SetValue( xmlInfo.GetFilepath(attrName) );
                ws = wf;
            }
            else if (attrType == XMLAttribute::Type::TString)
            {
                AttrWidgetString *wss = static_cast<AttrWidgetString*>(ws);
                wss->SetValue( xmlInfo.GetString(attrName) );
                ws = wss;
            }
            else if (attrType == XMLAttribute::Type::TEnum)
            {
                AttrWidgetEnum *we = static_cast<AttrWidgetEnum*>(ws);
                we->SetValue(attribute.GetEnumSelectedIndex());
            }

            ws->show();
        }
    }
}

void InspectorWidget::CreateWidgetSlots(XMLNode &xmlInfo)
{
    m_tagName = xmlInfo.GetTagName();

    for (auto itAttr : xmlInfo.GetAttributes())
    {
        XMLAttribute attribute = itAttr.second;
        m_attributes.push_back(attribute);
        if (!attribute.HasProperty(XMLProperty::Hidden))
        {
            std::string attrName  = attribute.GetName();
            XMLAttribute::Type attrType = attribute.GetType();
            AttributeWidget *ws = nullptr;
            if (attribute.HasVectoredType())
            {
                int numFields = attribute.GetNumberOfFieldsOfType();
                ws = new AttrWidgetVectorFloat(attrName, numFields, this);
            }
            else if (attrType == XMLAttribute::Type::TFile)
            {
                std::string fileExtension =
                        attribute.GetPropertyValue(XMLProperty::FileExtension.GetName());
                ws = new AttrWidgetFile(attrName, fileExtension, this);
            }
            else if (attrType == XMLAttribute::Type::TString)
            {
                bool readonly = attribute.HasProperty(XMLProperty::Readonly);
                bool inlined = attribute.HasProperty(XMLProperty::Inline);
                ws = new AttrWidgetString(attrName, this, readonly, inlined);
            }
            else if (attrType == XMLAttribute::Type::TEnum)
            {
                ws = new AttrWidgetEnum(attrName,
                                        xmlInfo.GetEnumNames(attrName), this);
            }

            if (ws)
            {
                m_attrNameToAttrWidget[attrName] = ws;
                layout()->addWidget(ws);
                ws->show();
            }
        }
    }
}

void InspectorWidget::_OnSlotValueChanged()
{
    NONULL(m_relatedInspectable);
    if (m_created)
    {
        XMLNode xmlInfo = GetWidgetXMLInfo();
        m_relatedInspectable->OnInspectorXMLChanged(&xmlInfo);
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
