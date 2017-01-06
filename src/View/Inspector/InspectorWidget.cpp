#include "InspectorWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Debug.h"
#include "Component.h"
#include "GameObject.h"
#include "EditorWindow.h"
#include "AttrWidgetEnum.h"
#include "AttrWidgetFile.h"
#include "AttrWidgetBool.h"
#include "AttrWidgetFloat.h"
#include "AttrWidgetColor.h"
#include "AttributeWidget.h"
#include "AttrWidgetButton.h"
#include "AttrWidgetString.h"
#include "WindowEventManager.h"
#include "AttrWidgetVectorFloat.h"


InspectorWidget::InspectorWidget()
{
}

void InspectorWidget::Init(const String &title, IInspectable *relatedInspectable)
{
    m_relatedInspectable = relatedInspectable;

    XMLNode xmlInfo = GetInspectableXMLInfo();
    ConstructFromWidgetXMLInfo(title, xmlInfo);

    setMinimumWidth(40);
    setAcceptDrops(true);
}

void InspectorWidget::ConstructFromWidgetXMLInfo(
        const String &title, XMLNode &xmlInfo, bool autoUpdate)
{
    Debug_Log("ConstructFromWidgetXMLInfo");

    m_vLayout = new QVBoxLayout();
        m_header = new QHBoxLayout();
            m_closeOpenButton = new QToolButton();
            m_titleLabel = new QLabel();
        m_gridLayout = new QGridLayout();
        m_gridLayout->setSpacing(0);
        m_gridLayout->setHorizontalSpacing(0);

    setLayout(m_vLayout);
    m_vLayout->addLayout(m_header, 0);
    m_vLayout->addLayout(m_gridLayout, 9999);

    m_header->setSpacing(5);
    m_header->setMargin(0);
    m_header->addWidget(m_closeOpenButton, 0, Qt::AlignLeft | Qt::AlignVCenter);
    m_header->addWidget(m_titleLabel, 99, Qt::AlignLeft | Qt::AlignVCenter);

    m_closeOpenButton->setStyleSheet("border: 0px; padding-left:-5px;  ");
    m_closeButtonPixmap.load(":/qss_icons/rc/branch_closed.png");
    m_openButtonPixmapap.load(":/qss_icons/rc/branch_open.png");
    UpdateCloseOpenButtonIcon();
    connect(m_closeOpenButton, SIGNAL(clicked()), this, SLOT(OnCloseOpenButtonClicked()));

    String fTitle = StringUtils::FormatInspectorLabel(title);
    m_titleLabel->setText(fTitle.ToQString());
    QFont font = m_titleLabel->font();
    font.setPixelSize(13);
    font.setBold(true);
    m_titleLabel->setFont(font);
    m_titleLabel->setAlignment(Qt::AlignLeft);

    CreateWidgetSlots(xmlInfo);
    RefreshWidgetValues(); // Initial catch of values

    m_created = true;

    if (autoUpdate)
    {
        //Every X seconds, update all the slots values
        connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(RefreshWidgetValues()));
        m_updateTimer.start(150);
    }
}

InspectorWidget::~InspectorWidget()
{
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
        String attrName = attribute.GetName();
        XMLAttribute::Type attrType = attribute.GetType();

        if (m_attrName_To_AttrWidget.ContainsKey(attrName))
        {
            AttributeWidget *aw = m_attrName_To_AttrWidget[attrName];
            if (attribute.HasVectoredType())
            {
                if (attrType == XMLAttribute::Type::Float)
                {
                    AttrWidgetFloat *wf = static_cast<AttrWidgetFloat*>(aw);
                    float v = wf->GetValue();
                    attribute.SetFloat(v, attribute.GetProperties());
                }
                else
                {
                    AttrWidgetVectorFloat *awv = static_cast<AttrWidgetVectorFloat*>(aw);
                    Array<float> v = awv->GetValue();
                    if (attrType == XMLAttribute::Type::Vector2)
                    {
                        attribute.SetVector2(Vector2(v[0], v[1]), attribute.GetProperties());
                    }
                    else if (attrType == XMLAttribute::Type::Vector3)
                    {
                        attribute.SetVector3(Vector3(v[0], v[1], v[2]), attribute.GetProperties());
                    }
                    else if (attrType == XMLAttribute::Type::Vector4 ||
                             attrType == XMLAttribute::Type::Quaternion ||
                             attrType == XMLAttribute::Type::Rect)
                    {
                        attribute.SetVector4(Vector4(v[0], v[1], v[2], v[3]), attribute.GetProperties());
                    }
                }
            }
            else if (attrType == XMLAttribute::Type::File)
            {
                AttrWidgetFile *awf = static_cast<AttrWidgetFile*>(aw);
                attribute.SetFilepath(awf->GetValue(),
                                      attribute.GetPropertyValue(XMLProperty::FileExtension.GetName()),
                                      attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::String)
            {
                AttrWidgetString *aws = static_cast<AttrWidgetString*>(aw);
                attribute.SetString(aws->GetValue(), attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::Bool)
            {
                AttrWidgetBool *wb = static_cast<AttrWidgetBool*>(aw);
                attribute.SetBool(wb->GetValue(), attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::Enum)
            {
                AttrWidgetEnum *awe = static_cast<AttrWidgetEnum*>(aw);
                attribute.SetEnum(attribute.GetEnumNames(), awe->GetValue(), // selected index
                                  attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::Color)
            {
                AttrWidgetColor *awc = static_cast<AttrWidgetColor*>(aw);
                attribute.SetColor(awc->GetValue(), attribute.GetProperties());
            }
        }
        xmlInfo.SetAttribute(attribute);
    }

    return xmlInfo;
}

QGridLayout *InspectorWidget::GetGridLayout() const
{
    return m_gridLayout;
}

int InspectorWidget::GetNextRowIndex() const
{
    return m_gridLayout->rowCount();
}

void InspectorWidget::SetTitle(const String &title)
{
    m_titleLabel->setText(QString::fromStdString(title));
}

void InspectorWidget::RefreshWidgetValues()
{
    XMLNode xmlInfo = GetInspectableXMLInfo();
    xmlInfo.SetTagName(m_tagName);
    bool hasToRefreshHard = false;
    for (auto itAttr : xmlInfo.GetAttributes())
    {
        XMLAttribute attribute = itAttr.second;
        String attrName  = attribute.GetName();
        if( m_attrName_To_AttrWidget.ContainsKey(attrName))
        {
            AttributeWidget *ws = m_attrName_To_AttrWidget[attrName];
            hasToRefreshHard = hasToRefreshHard ||
                    (ws->isHidden() != attribute.HasProperty(XMLProperty::Hidden));
            ws->Refresh(attribute);
        }
    }

    if (hasToRefreshHard)
    {
        //Inspector::GetInstance()->RefreshHard();
    }
}

void InspectorWidget::CreateWidgetSlots(XMLNode &xmlInfo)
{
    m_tagName = xmlInfo.GetTagName();

    for (auto itAttr : xmlInfo.GetAttributes())
    {
        XMLAttribute attribute = itAttr.second;
        m_attributes.PushBack(attribute);
        AttributeWidget *w = AttributeWidget::FromXMLAttribute(attribute, this);
        if (w)
        {
            m_attrName_To_AttrWidget[attribute.GetName()] = w;
            w->adjustSize();
        }
    }
}

void InspectorWidget::_OnSlotValueChanged()
{
    ASSERT(m_relatedInspectable);
    if (m_created)
    {
        XMLNode xmlInfo = GetWidgetXMLInfo();
        m_relatedInspectable->OnInspectorXMLChanged(&xmlInfo);
    }
    WindowEventManager::GetInstance()->NotifyInspectorSlotChanged(this);
}

void InspectorWidget::OnCloseOpenButtonClicked()
{
    m_closed = !m_closed;
    SetClosed(m_closed);
    Inspector::GetInstance()->RefreshHard();
}

void InspectorWidget::SetClosed(bool closedWidget)
{
    for (int i = 0; i < m_gridLayout->count(); ++i)
    {
        QLayoutItem *item = m_gridLayout->itemAt(i);
        if(item->widget())
        {
            item->widget()->setHidden(closedWidget);
        }
    }
    UpdateCloseOpenButtonIcon();
}

void InspectorWidget::UpdateCloseOpenButtonIcon()
{
    if (m_closed)
    {
        m_closeOpenButton->setIcon(QIcon(m_closeButtonPixmap));
    }
    else
    {
        m_closeOpenButton->setIcon(QIcon(m_openButtonPixmapap));
    }
}

void InspectorWidget::_OnSlotValueChanged(int _)
{
    _OnSlotValueChanged();
}
void InspectorWidget::_OnSlotValueChanged(double _)
{
    _OnSlotValueChanged();
}
void InspectorWidget::_OnSlotValueChanged(QString _)
{
    _OnSlotValueChanged();
}
