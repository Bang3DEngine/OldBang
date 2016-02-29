#include "InspectorPartSlotWidget.h"

InspectorPartSlotWidget::InspectorPartSlotWidget(InspectorPartWidget *parent) : QWidget(nullptr), parent(parent)
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void InspectorPartSlotWidget::focusInEvent(QFocusEvent *event)
{
    Logger_Log("AAAA");
}

void InspectorPartSlotWidget::focusOutEvent(QFocusEvent *event)
{
    Logger_Log("BBBB");
}
