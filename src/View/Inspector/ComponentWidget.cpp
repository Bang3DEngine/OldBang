#include "ComponentWidget.h"

#include "Component.h"
#include "GameObject.h"
#include "EditorWindow.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(), m_cwContextMenu(this)
{
    Init(relatedComponent->GetName(), relatedComponent);

    m_relatedComponent = relatedComponent;

    if (relatedComponent->GetName() != "Transform")
    {
        m_enabledCheckbox = new QCheckBox();
        m_enabledCheckbox->setChecked(m_relatedComponent->IsEnabled());
        connect(m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_gridLayout->addWidget(m_enabledCheckbox, 0, 1, 1, 1,
                                Qt::AlignRight | Qt::AlignVCenter);
    }
}

ComponentWidget::~ComponentWidget()
{
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    if (m_relatedComponent)
    {
        m_relatedComponent->SetEnabled(checked);
    }
}

