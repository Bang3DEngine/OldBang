#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include "Array.h"

#include <QTimer>
#include <QFocusEvent>
#include <QComboBox>

#include "AttributeWidget.h"

class AttrWidgetEnum  : public AttributeWidget
{
private:
    QComboBox *m_comboBox = nullptr;

public:
    AttrWidgetEnum(const XMLAttribute &xmlAttribute,
                   InspectorWidget *m_inspectorWidget);

    virtual void SetValue(int index);
    virtual int GetValue();

    virtual void Refresh(const XMLAttribute &attribute) override;
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
