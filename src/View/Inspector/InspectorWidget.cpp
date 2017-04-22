#include "Bang/InspectorWidget.h"

#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/Inspector.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/EditorWindow.h"
#include "Bang/AttrWidgetInt.h"
#include "Bang/AttrWidgetEnum.h"
#include "Bang/AttrWidgetFile.h"
#include "Bang/AttrWidgetBool.h"
#include "Bang/AttrWidgetFloat.h"
#include "Bang/AttrWidgetColor.h"
#include "Bang/AttributeWidget.h"
#include "Bang/AttrWidgetButton.h"
#include "Bang/AttrWidgetString.h"
#include "Bang/WindowEventManager.h"
#include "Bang/AttrWidgetVectorFloat.h"


InspectorWidget::InspectorWidget()
    : DragDropQWidget(Inspector::GetInstance())
{
}

void InspectorWidget::Init(const String &title,
                           SerializableObject *relatedInspectable)
{
    m_relatedInspectable = relatedInspectable;

    XMLNode xmlInfo = GetInspectableXMLInfo();
    ConstructFromWidgetXMLInfo(title, xmlInfo);

    SetIcon( relatedInspectable->GetIcon() );

    setAcceptDrops(true);
    RefreshWidgetValues();
}

void InspectorWidget::ConstructFromWidgetXMLInfo(
        const String &title, XMLNode &xmlInfo, bool autoUpdate)
{
    setVisible(false);

    m_vLayout.addLayout(&m_headerLayout, 0);
    m_vLayout.addLayout(&m_gridLayout, 99);

    m_headerLayout.setSpacing(5);
    m_headerLayout.addWidget(&m_closeOpenButton, 0,
                             Qt::AlignLeft | Qt::AlignVCenter);
    m_headerLayout.addWidget(&m_titleLabel,     99,
                             Qt::AlignLeft | Qt::AlignVCenter);

    m_gridLayout.setSpacing(0);

    m_closeOpenButton.setStyleSheet(
                "padding:0px; border: 0px; margin-left:-5px;");
    QObject::connect(&m_closeOpenButton, SIGNAL(clicked()),
                     this, SLOT(OnCloseOpenButtonClicked()));
    UpdateCloseOpenButtonIcon();

    String fTitle = Inspector::FormatInspectorLabel(title);
    m_titleLabel.setText(fTitle.ToQString());
    QFont font = m_titleLabel.font();
    font.setPixelSize(13);
    font.setBold(true);
    m_titleLabel.setFont(font);
    m_titleLabel.setAlignment(Qt::AlignLeft);

    CreateWidgetSlots(xmlInfo);
    RefreshWidgetValues();

    if (autoUpdate)
    {
        // To refresh all the slots values
        QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                         this, SLOT(RefreshWidgetValues()));
        m_refreshTimer.start(50);
    }

    setLayout(&m_vLayout);
    m_created = true;
    setVisible(true);
}

InspectorWidget::~InspectorWidget()
{
    m_relatedInspectable = nullptr;
    m_refreshTimer.stop();
    QObject::disconnect(&m_refreshTimer, SIGNAL(timeout()),
                        this, SLOT(RefreshWidgetValues()));
}

XMLNode InspectorWidget::GetInspectableXMLInfo() const
{
    XMLNode xmlInfo;
    if (m_relatedInspectable)
    {
        m_relatedInspectable->Write(&xmlInfo);
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

QGridLayout *InspectorWidget::GetGridLayout()
{
    return &m_gridLayout;
}

int InspectorWidget::GetNextRowIndex() const
{
    return m_gridLayout.rowCount();
}

void InspectorWidget::SetTitle(const String &title)
{
    m_titleLabel.setText(title.ToQString());
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

void InspectorWidget::SetIcon(const QPixmap &icon)
{
    QPixmap pm = icon.scaled(16, 16, Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation);
    m_iconLabel.setPixmap(pm);
    m_headerLayout.insertWidget(1, &m_iconLabel, 0, Qt::AlignCenter);
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
    ENSURE(m_relatedInspectable);
    if (m_created) { m_relatedInspectable->Read( GetWidgetXMLInfo() ); }
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
    for (int i = 0; i < m_gridLayout.count(); ++i)
    {
        QLayoutItem *item = m_gridLayout.itemAt(i);
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
    m_vLayout.setContentsMargins(c_marginLeft,  c_marginTop,
                                  c_marginRight, c_marginBot);
}

void InspectorWidget::UpdateCloseOpenButtonIcon()
{
    static QPixmap closeButtonPixmap(":/qss_icons/rc/branch_closed.png");
    static QPixmap openButtonPixmap(":/qss_icons/rc/branch_open.png");

    const QPixmap &pixmap = m_closed ? closeButtonPixmap : openButtonPixmap;
    m_closeOpenButton.setIcon( QIcon(pixmap) );
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
