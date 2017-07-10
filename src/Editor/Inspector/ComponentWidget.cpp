#include "Bang/ComponentWidget.h"

#include "Bang/Chrono.h"
#include "Bang/Component.h"
#include "Bang/IconManager.h"
#include "Bang/EditorWindow.h"
#include "Bang/ComponentInspectable.h"

ComponentWidget::ComponentWidget(Component *relatedComponent) :
    InspectorWidget(),
    m_iconLabel(this),
    m_cwContextMenu(this)
{
    p_component = relatedComponent;
    m_componentInspectable = new Inspectable<Component>(p_component);
    InspectorWidget::Init(m_componentInspectable);

    m_closed = p_component->IsClosedInInspector();
    SetClosed(m_closed);
    UpdateCloseOpenButtonIcon();

    if (!p_component->GetClassName().Contains("Transform"))
    {
        m_enabledCheckbox.setChecked(p_component->IsEnabled(false));
        connect(&m_enabledCheckbox, SIGNAL(clicked(bool)),
                this, SLOT(OnEnabledCheckboxPressed(bool)));
        m_headerLayout.addWidget(&m_enabledCheckbox, 0,
                                 Qt::AlignRight | Qt::AlignVCenter);
    }
    RefreshWidgetValues();
}

ComponentWidget::~ComponentWidget()
{
    delete m_componentInspectable;
}

int ComponentWidget::GetHeightSizeHint()
{
    return InspectorWidget::GetHeightSizeHint();
}

void ComponentWidget::OnEnabledCheckboxPressed(bool checked)
{
    ENSURE(p_component);
    p_component->SetEnabled(checked);
}

void ComponentWidget::SetClosed(bool closed)
{
    ENSURE(p_component);
    InspectorWidget::SetClosed(closed);
    p_component->SetClosedInInspector(closed);
}

