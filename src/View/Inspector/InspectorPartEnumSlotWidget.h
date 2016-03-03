#ifndef INSPECTORPARTENUMSLOTWIDGET_H
#define INSPECTORPARTENUMSLOTWIDGET_H

#include <vector>
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
    InspectorPartEnumSlotWidget(const std::string &labelString,
                                const std::vector<std::string> &initialValue,
                                const int selectedValue,
                                InspectorPartWidget *parent);

    virtual void SetValue(int index);
    virtual int GetValue();
};

#endif // INSPECTORPARTENUMSLOTWIDGET_H
