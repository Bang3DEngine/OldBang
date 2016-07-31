#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include <vector>
#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QComboBox>

#include "AttributeWidget.h"

class AttrWidgetEnum  : public AttributeWidget
{
private:
    QComboBox *m_comboBox = nullptr;

public:
    AttrWidgetEnum(const std::string &labelString,
                   const std::vector<std::string> &enumNames,
                   InspectorWidget *m_parent);

    virtual void SetValue(int index);
    virtual int GetValue();
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
