#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include <QTimer>
#include <QComboBox>

#include "XMLAttribute.h"
#include "InspectorWidget.h"
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
