#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include <vector>
#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QComboBox>

#include "InspectorSW.h"

class InspectorEnumSW  : public InspectorSW
{
private:
    QComboBox *p_comboBox = nullptr;

public:
    InspectorEnumSW(const std::string &labelString,
                                const std::vector<std::string> &initialValue,
                                const int selectedValue,
                                InspectorWidget *p_parent);

    virtual void SetValue(int index);
    virtual int GetValue();
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
