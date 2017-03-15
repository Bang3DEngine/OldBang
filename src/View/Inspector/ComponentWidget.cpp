#include "ComponentWidget.h"

#include "Chrono.h"
#include "Component.h"
#include "EditorWindow.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(), m_cwContextMenu(this)
{
    Chrono c;
    c.MarkEvent("Construct ComponentWidget " + relatedComponent->GetName());
    p_relatedComponent = relatedComponent;
    Init(p_relatedComponent->GetName(), p_relatedComponent);

    m_closed = p_relatedComponent->IsClosedInInspector();
    SetClosed(m_closed);
    UpdateCloseOpenButtonIcon();

    if (!p_relatedComponent->GetName().Contains("Transform"))
    {
        m_enabledCheckbox.setChecked(p_relatedComponent->IsEnabled());
        connect(&m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_header.addWidget(&m_enabledCheckbox, 0,
                           Qt::AlignRight | Qt::AlignVCenter);
    }
    RefreshWidgetValues();
    c.Log();
}

ComponentWidget::~ComponentWidget()
{
}

int ComponentWidget::GetHeightSizeHint()
{
    return InspectorWidget::GetHeightSizeHint();
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    ASSERT(p_relatedComponent);
    p_relatedComponent->SetEnabled(checked);
}

void ComponentWidget::SetClosed(bool closed)
{
    ASSERT(p_relatedComponent);
    InspectorWidget::SetClosed(closed);
    p_relatedComponent->SetClosedInInspector(closed);
}

