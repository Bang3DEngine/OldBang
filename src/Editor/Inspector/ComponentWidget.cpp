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
    p_component = relatedComponent;
    p_component->RegisterDestroyListener(this);
}

ComponentWidget::~ComponentWidget()
{
}
void ComponentWidget::OnDestroy()
{
    InspectorWidget::OnDestroy();
}

void ComponentWidget::InitExtra()
{
    InspectorWidget::InitExtra();

    SetIcon(IconManager::GetIcon(p_component->GetClassName()));
    SetTitle(p_component->GetClassName());

    SetClosed(false);
    UpdateCloseOpenButtonIcon();

    m_enabledCheckbox.setChecked(p_component->IsEnabled(false));
    connect(&m_enabledCheckbox, SIGNAL(clicked(bool)),
            this, SLOT(OnEnabledCheckboxPressed(bool)));
    m_headerLayout.addWidget(&m_enabledCheckbox, 0,
                             Qt::AlignRight | Qt::AlignVCenter);
}

int ComponentWidget::GetHeightSizeHint() const
{
    return InspectorWidget::GetHeightSizeHint();
}

void ComponentWidget::SetEnabledCheckboxVisible(bool visible)
{
    m_enabledCheckbox.setVisible(visible);
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    p_component->SetEnabled(checked);
    emit Changed(this);
}

void ComponentWidget::SetClosed(bool closed)
{
    InspectorWidget::SetClosed(closed);
}

void ComponentWidget::OnDestroyableDestroyed(Destroyable *destroyedObject)
{
    if (destroyedObject == p_component) { DemandDestroy(); }
}
