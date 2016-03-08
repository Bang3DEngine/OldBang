#ifndef INSPECTORPARTENUMSLOTWIDGET_H
#define INSPECTORPARTENUMSLOTWIDGET_H

#include <vector>
#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QComboBox>

#include "InspectorPartSlotWidget.h"

template<>
class InspectorPartSlotWidget< std::vector <std::string> >  : public InspectorPartSlotWidgetBase
{
private:
    QComboBox *comboBox = nullptr;

public:
    InspectorPartSlotWidget< std::vector <std::string> >(
                                InspectorPartWidget *parent,
                                const std::string &label,
                                const std::vector<std::string> &value,
                                int selectedValue);

    int GetSelectedValueIndex() { return comboBox->currentIndex(); }
};

#endif // INSPECTORPARTENUMSLOTWIDGET_H
