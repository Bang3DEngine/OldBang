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
#include "Bang/AttributeWidget.h"
#include "Bang/WindowEventManager.h"
#include "Bang/SerializableObject.h"

InspectorWidget::InspectorWidget() : DragDropQWidget(nullptr)
{
}

void InspectorWidget::Init()
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

    m_closeOpenButton.setStyleSheet("padding:0px;border:0px;margin-left:-5px;");
    QObject::connect(&m_closeOpenButton, SIGNAL(clicked()),
                     this, SLOT(OnCloseOpenButtonClicked()));
    UpdateCloseOpenButtonIcon();

    String title = "NAME_NOT_SET";
    m_titleLabel.setText(title.ToQString());
    QFont font = m_titleLabel.font();
    font.setPixelSize(13);
    font.setBold(true);
    m_titleLabel.setFont(font);
    m_titleLabel.setAlignment(Qt::AlignLeft);

    InitExtra();

    QObject::connect(&m_refreshTimer, SIGNAL(timeout()),
                     this, SLOT(Refresh()));
    m_refreshTimer.start(10);

    setLayout(&m_vLayout);
    m_created = true;

    setAcceptDrops(true);
    setVisible(true);
}

InspectorWidget::~InspectorWidget()
{
    OnDestroy();
}

void InspectorWidget::OnDestroy()
{
    m_refreshTimer.stop();
    QObject::disconnect(&m_refreshTimer, SIGNAL(timeout()),
                        this, SLOT(Refresh()));
}

QGridLayout *InspectorWidget::GetGridLayout()
{
    return &m_gridLayout;
}

void InspectorWidget::SetTitle(const String &title)
{
    m_titleLabel.setText(title.ToQString());
}

bool InspectorWidget::IsClosed() const
{
    return m_closed;
}

int InspectorWidget::GetHeightSizeHint() const
{
    int heightSizeHint = 0;
    heightSizeHint += 50; // Header height

    // Add up children widget's height size hints
    for (AttributeWidget *aw : m_attributeWidgets)
    {
        heightSizeHint += aw->IsVisible() ? aw->GetHeightSizeHint() : 0;
    }

    return heightSizeHint;
}

void InspectorWidget::Refresh()
{
    UpdateContentMargins();
}

void InspectorWidget::InitExtra()
{
}

void InspectorWidget::SetIcon(const QPixmap &icon)
{
    QPixmap pm = icon.scaled(16, 16, Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation);
    m_iconLabel.setPixmap(pm);
    m_headerLayout.insertWidget(1, &m_iconLabel, 0, Qt::AlignCenter);
}

void InspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget* attrWidget)
{
}

void InspectorWidget::OnCloseOpenButtonClicked()
{
    m_closed = !m_closed;
    SetClosed(m_closed);
    Refresh();
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

void InspectorWidget::InsertAttributeWidget(AttributeWidget *attrWidget,
                                            int _index)
{
    const int index = (_index >= 0 ? _index : m_gridLayout.rowCount());
    m_gridLayout.addWidget(attrWidget, index, 0);
    QObject::connect(attrWidget, SIGNAL(OnValueChanged(IAttributeWidget*)),
                     this, SLOT(OnAttrWidgetValueChanged(IAttributeWidget*)));

    m_attributeWidgets.PushBack(attrWidget);
}

void InspectorWidget::UpdateContentMargins()
{
    Inspector *insp = Inspector::GetInstance();
    bool scrollbarVisible = insp->verticalScrollBar()->isVisible();
    const int c_marginLeft  = 10;
    const int c_marginTop = IsClosed() ? 0 : 5;
    const int c_marginRight = scrollbarVisible ? 20 : 5;
    const int c_marginBot = IsClosed() ? 0 : 15;
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
