#include "InspectorWidget.h"

#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Debug.h"
#include "Inspector.h"
#include "Component.h"
#include "GameObject.h"
#include "EditorWindow.h"
#include "AttrWidgetInt.h"
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
    : DragDropQWidget(Inspector::GetInstance())
{
}

void InspectorWidget::Init(const String &title, IInspectable *relatedInspectable)
{
    m_relatedInspectable = relatedInspectable;
    XMLNode xmlInfo = GetInspectableXMLInfo();
    ConstructFromWidgetXMLInfo(title, xmlInfo);
    setAcceptDrops(true);
    RefreshWidgetValues();
}

void InspectorWidget::ConstructFromWidgetXMLInfo(
        const String &title, XMLNode &xmlInfo, bool autoUpdate)
{
    m_vLayout = new QVBoxLayout();
        m_header = new QHBoxLayout();
            m_closeOpenButton = new QToolButton();
            m_titleLabel = new QLabel();
        m_gridLayout = new QGridLayout();

    m_vLayout->addLayout(m_header, 0);
    m_vLayout->addLayout(m_gridLayout, 99);

    m_header->setSpacing(5);
    m_header->addWidget(m_closeOpenButton, 0, Qt::AlignLeft | Qt::AlignVCenter);
    m_header->addWidget(m_titleLabel,     99, Qt::AlignLeft | Qt::AlignVCenter);

    m_gridLayout->setSpacing(0);

    m_closeOpenButton->setStyleSheet("padding:0px; border: 0px; margin-left:-5px;");
    m_closeButtonPixmap.load(":/qss_icons/rc/branch_closed.png");
    m_openButtonPixmap.load(":/qss_icons/rc/branch_open.png");
    QObject::connect(m_closeOpenButton, SIGNAL(clicked()),
                     this, SLOT(OnCloseOpenButtonClicked()));
    UpdateCloseOpenButtonIcon();

    String fTitle = Inspector::FormatInspectorLabel(title);
    m_titleLabel->setText(fTitle.ToQString());
    QFont font = m_titleLabel->font();
    font.setPixelSize(13);
    font.setBold(true);
    m_titleLabel->setFont(font);
    m_titleLabel->setAlignment(Qt::AlignLeft);

    CreateWidgetSlots(xmlInfo);
    RefreshWidgetValues();

    if (autoUpdate)
    {
        // To refresh all the slots values
        QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                         this, SLOT(RefreshWidgetValues()));
        m_refreshTimer.start(10);
    }

    setLayout(m_vLayout);
    m_created = true;
}

InspectorWidget::~InspectorWidget()
{
    m_refreshTimer.stop();
    QObject::disconnect(&m_refreshTimer, SIGNAL(timeout()),
                        this, SLOT(RefreshWidgetValues()));
}

XMLNode InspectorWidget::GetInspectableXMLInfo() const
{
    XMLNode xmlInfo;
    if (m_relatedInspectable)
    {
        m_relatedInspectable->OnInspectorXMLNeeded(&xmlInfo);
    }
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
                    AttrWidgetFloat *wf = Object::SCast<AttrWidgetFloat>(aw);
                    float v = wf->GetValue();
                    attribute.SetFloat(v, attribute.GetProperties());
                }
                else if (attrType == XMLAttribute::Type::Int)
                {
                    AttrWidgetInt *wi = Object::SCast<AttrWidgetInt>(aw);
                    int v = wi->GetValue();
                    attribute.SetInt(v, attribute.GetProperties());
                }
                else
                {
                    AttrWidgetVectorFloat *awv =
                            Object::SCast<AttrWidgetVectorFloat>(aw);
                    Array<float> v = awv->GetValue();
                    if (attrType == XMLAttribute::Type::Vector2)
                    {
                        attribute.SetVector2(Vector2(v[0], v[1]),
                                             attribute.GetProperties());
                    }
                    else if (attrType == XMLAttribute::Type::Vector3)
                    {
                        attribute.SetVector3(Vector3(v[0], v[1], v[2]),
                                             attribute.GetProperties());
                    }
                    else if (attrType == XMLAttribute::Type::Vector4 ||
                             attrType == XMLAttribute::Type::Quaternion ||
                             attrType == XMLAttribute::Type::Rect)
                    {
                        attribute.SetVector4(Vector4(v[0], v[1], v[2], v[3]),
                                             attribute.GetProperties());
                    }
                }
            }
            else if (attrType == XMLAttribute::Type::File)
            {
                AttrWidgetFile *awf = Object::SCast<AttrWidgetFile>(aw);
                attribute.SetFilepath(
                    awf->GetValue(),
                    attribute.GetPropertyValue(XMLProperty::FileExtension
                                               .GetName()),
                    attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::String)
            {
                AttrWidgetString *aws = Object::SCast<AttrWidgetString>(aw);
                attribute.SetString(aws->GetValue(), attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::Bool)
            {
                AttrWidgetBool *wb = Object::SCast<AttrWidgetBool>(aw);
                attribute.SetBool(wb->GetValue(), attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::Enum)
            {
                AttrWidgetEnum *awe = Object::SCast<AttrWidgetEnum>(aw);
                // Selected index<
                attribute.SetEnum(attribute.GetEnumNames(), awe->GetValue(),
                                  attribute.GetProperties());
            }
            else if (attrType == XMLAttribute::Type::Color)
            {
                AttrWidgetColor *awc = Object::SCast<AttrWidgetColor>(aw);
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
    m_titleLabel->setText(title.ToQString());
}

bool InspectorWidget::IsClosed() const
{
    return m_closed;
}

int InspectorWidget::GetHeightSizeHint()
{
    int heightSizeHint = 0;
    heightSizeHint += 60; // Header height

    // Add up children widget's height size hints
    typedef std::pair<String, AttributeWidget*> Pair;
    for (Pair name_AttrWidget : m_attrName_To_AttrWidget)
    {
        AttributeWidget *attrWidget = name_AttrWidget.second;
        heightSizeHint += attrWidget->GetHeightSizeHint();
    }

    return heightSizeHint;
}

void InspectorWidget::OnDestroy()
{
    m_refreshTimer.stop();
    QObject::disconnect(&m_refreshTimer, SIGNAL(timeout()),
                        this, SLOT(RefreshWidgetValues()));
    m_relatedInspectable = nullptr;
}

void InspectorWidget::RefreshWidgetValues()
{
    XMLNode xmlInfo = GetInspectableXMLInfo();
    xmlInfo.SetTagName(m_tagName);
    for (auto itAttr : xmlInfo.GetAttributesListInOrder())
    {
        XMLAttribute attribute = itAttr.second;
        String attrName = attribute.GetName();
        if( m_attrName_To_AttrWidget.ContainsKey(attrName))
        {
            AttributeWidget *ws = m_attrName_To_AttrWidget[attrName];
            ws->Refresh(attribute);
        }
        else
        {
            // New attribute detected when refreshing
        }
    }
    UpdateContentMargins();
}

void InspectorWidget::CreateWidgetSlots(XMLNode &xmlInfo)
{
    m_tagName = xmlInfo.GetTagName();
    for (auto itAttr : xmlInfo.GetAttributesListInOrder())
    {
        XMLAttribute attribute = itAttr.second;
        m_attributes.PushBack(attribute);
        AttributeWidget *w = AttributeWidget::FromXMLAttribute(attribute, this);
        if (w)
        {
            m_attrName_To_AttrWidget[attribute.GetName()] = w;
            if (attribute.HasProperty(XMLProperty::Hidden)) { w->hide(); }
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
    RefreshWidgetValues();
    UpdateContentMargins();
    Inspector::GetInstance()->RefreshSizeHints();
}

void InspectorWidget::SetClosed(bool closedWidget)
{
    for (int i = 0; i < m_gridLayout->count(); ++i)
    {
        QLayoutItem *item = m_gridLayout->itemAt(i);
        if (!item->widget()) { continue; }
        item->widget()->setHidden(closedWidget);
    }
    UpdateCloseOpenButtonIcon();
}

void InspectorWidget::UpdateContentMargins()
{
    Inspector *insp = Inspector::GetInstance();
    bool scrollbarVisible = insp->verticalScrollBar()->isVisible();
    const int c_marginLeft  = 10;
    const int c_marginTop = IsClosed() ? 0 : 10;
    const int c_marginRight = scrollbarVisible ? 20 : 5;
    const int c_marginBot = IsClosed() ? 0 : 20;
    m_vLayout->setContentsMargins(c_marginLeft,  c_marginTop,
                                  c_marginRight, c_marginBot);
}

void InspectorWidget::UpdateCloseOpenButtonIcon()
{
    const QPixmap &pixmap = m_closed ? m_closeButtonPixmap :
                                       m_openButtonPixmap;
    m_closeOpenButton->setIcon( QIcon(pixmap) );
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
