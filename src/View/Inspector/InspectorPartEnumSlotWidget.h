#ifndef INSPECTORPARTENUMSLOTWIDGET_H
#define INSPECTORPARTENUMSLOTWIDGET_H

#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QComboBox>

#include "InspectorPartSlotWidget.h"

class InspectorPartEnumSlotWidget  : public InspectorPartSlotWidget
{
private:
    QComboBox *comboBox = nullptr;

public:
    InspectorPartEnumSlotWidget(float initialValue,
                                const std::string &labelString,
                                InspectorPartWidget *parent);
};

#endif // INSPECTORPARTENUMSLOTWIDGET_H
