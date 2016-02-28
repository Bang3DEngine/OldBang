#include "InspectorPartSlotWidget.h"

InspectorPartSlotWidget::InspectorPartSlotWidget(InspectorPartWidget *parent) : QWidget(nullptr), parent(parent) {}

void InspectorPartSlotWidget::focusInEvent(QFocusEvent *event)
{
    Logger_Log("Focus in");
}

void InspectorPartSlotWidget::focusOutEvent(QFocusEvent *event)
{
    Logger_Log("Focus out");
}


