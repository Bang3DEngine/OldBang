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

    m_nameLabel = new QLabel();
    m_nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_horizontalLayout.addWidget(m_nameLabel);

    setLayout(&m_horizontalLayout);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    // setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    SetHeightSizeHint(25);
    setVisible(true);
}

AttributeWidget::~AttributeWidget()
{
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
    if (visible && !IsVisible())
    {
        setVisible(true);
        m_nameLabel->setVisible(true);
    }
    else if (!visible && IsVisible())
    {
        hide();
        m_nameLabel->hide();
    }
}

bool AttributeWidget::IsVisible() const
{
    return !isHidden();
}

void AttributeWidget::DettachNameLabel()
{
    m_horizontalLayout.removeWidget(m_nameLabel);
}

QLabel *AttributeWidget::GetNameLabel() const
{
    return m_nameLabel;
}

void AttributeWidget::SetLabelText(const String &labelText)
{
    m_nameLabel->setText(labelText.ToQString());
}

void AttributeWidget::SetLabelColor(const Color &labelColor)
{
    m_nameLabel->setStyleSheet(
          "color: " + labelColor.ToQColor().name(QColor::NameFormat::HexRgb));
}
