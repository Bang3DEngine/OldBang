#ifndef INSPECTORPARTENUMSLOTWIDGET_H
#define INSPECTORPARTENUMSLOTWIDGET_H

#include <vector>
#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QComboBox>

#include "InspectorSW.h"

class InspectorEnumSW  : public InspectorSW
{
private:
    QComboBox *comboBox = nullptr;

public:
    InspectorEnumSW(const std::string &labelString,
                                const std::vector<std::string> &initialValue,
                                const int selectedValue,
                                InspectorWidget *parent);

    virtual void SetValue(int index);
    virtual int GetValue();
};

#endif // INSPECTORPARTENUMSLOTWIDGET_H
