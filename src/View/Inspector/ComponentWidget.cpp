#include "ComponentWidget.h"

#include "Component.h"
#include "EditorWindow.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(), m_cwContextMenu(this)
{
    Init(relatedComponent->GetName(), relatedComponent);

    m_relatedComponent = relatedComponent;

    m_closed = m_relatedComponent->IsClosedInInspector();
    SetClosed(m_closed);
    UpdateCloseOpenButtonIcon();
    Debug_Log("Creating component " << m_relatedComponent->GetName() << ": " << m_closed);
    if (relatedComponent->GetName() != "Transform")
    {
        m_enabledCheckbox = new QCheckBox();
        m_enabledCheckbox->setChecked(m_relatedComponent->IsEnabled());
        connect(m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_header->addWidget(m_enabledCheckbox, 0, Qt::AlignRight | Qt::AlignVCenter);
    }
}

ComponentWidget::~ComponentWidget()
{
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

