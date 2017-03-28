#include "Bang/ComponentWidget.h"

#include "Bang/Chrono.h"
#include "Bang/Component.h"
#include "Bang/IconManager.h"
#include "Bang/EditorWindow.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(),
    m_iconLabel(this),
    m_cwContextMenu(this)
{
    Chrono c;
    c.MarkEvent("Construct ComponentWidget " + relatedComponent->GetClassName());
    p_component = relatedComponent;
    InspectorWidget::Init(p_component->GetClassName(), p_component);

    m_closed = p_component->IsClosedInInspector();
    SetClosed(m_closed);
    UpdateCloseOpenButtonIcon();

    if (!p_component->GetClassName().Contains("Transform"))
    {
        m_enabledCheckbox.setChecked(p_component->IsEnabled());
        connect(&m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_headerLayout.addWidget(&m_enabledCheckbox, 0,
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
    ASSERT(p_component);
    p_component->SetEnabled(checked);
}

void ComponentWidget::SetClosed(bool closed)
{
    ASSERT(p_component);
    InspectorWidget::SetClosed(closed);
    p_component->SetClosedInInspector(closed);
}

