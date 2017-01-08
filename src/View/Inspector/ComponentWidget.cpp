#include "ComponentWidget.h"

#include "Component.h"
#include "EditorWindow.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(), m_cwContextMenu(this)
{
    m_relatedComponent = relatedComponent;
    Start();
}

ComponentWidget::~ComponentWidget()
{
}

int ComponentWidget::GetHeightSizeHint()
{
    return InspectorWidget::GetHeightSizeHint();
}

void ComponentWidget::Start()
{
    Init(m_relatedComponent->GetName(), m_relatedComponent);

    m_closed = m_relatedComponent->IsClosedInInspector();
    SetClosed(m_closed);
    UpdateCloseOpenButtonIcon();
    if (m_relatedComponent->GetName() != "Transform")
    {
        m_enabledCheckbox = new QCheckBox();
        m_enabledCheckbox->setChecked(m_relatedComponent->IsEnabled());
        connect(m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_header->addWidget(m_enabledCheckbox, 0, Qt::AlignRight | Qt::AlignVCenter);
    }
    RefreshWidgetValues();
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    ASSERT(m_relatedComponent);
    m_relatedComponent->SetEnabled(checked);
}

void ComponentWidget::SetClosed(bool closed)
{
    ASSERT(m_relatedComponent);
    InspectorWidget::SetClosed(closed);
    m_relatedComponent->SetClosedInInspector(closed);
}

