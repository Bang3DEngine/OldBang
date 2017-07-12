#include "Bang/AttributeWidget.h"

#include <QLabel>
#include <QBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/Inspector.h"

AttributeWidget::AttributeWidget()
{
    setVisible(false);

    m_horizontalLayout.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_horizontalLayout.setContentsMargins(5,0,0,0);

    setLayout(&m_horizontalLayout);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    SetHeightSizeHint(25);
    setVisible(true);
}

AttributeWidget::~AttributeWidget()
{
}

void AttributeWidget::AfterConstructor()
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void AttributeWidget::OnValueChanged()
{
    emit OnValueChanged(this);
}

void AttributeWidget::Refresh()
{
}

int AttributeWidget::GetHeightSizeHint() const
{
    if (!IsVisible()) { return 0; }
    return m_heightSizeHint;
}

void AttributeWidget::SetHeightSizeHint(int heightSizeHint)
{
    setFixedHeight(heightSizeHint);
    m_heightSizeHint = heightSizeHint;
}

void AttributeWidget::SetVisible(bool visible)
{
    if (visible && !IsVisible()) { setVisible(true); }
    else if (!visible && IsVisible()) { hide(); }
}

bool AttributeWidget::IsVisible() const
{
    return !isHidden();
}

void AttributeWidget::CreateLabel(const String &labelText)
{
    if (!labelText.Empty())
    {
        m_nameLabel = new QLabel(labelText.ToQString());
        m_nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_horizontalLayout.insertWidget(0, m_nameLabel);
    }
}
