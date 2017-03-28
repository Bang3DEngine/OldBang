#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include <QComboBox>

#include "Bang/XMLAttribute.h"
#include "Bang/InspectorWidget.h"
#include "Bang/AttributeWidget.h"

class ComboBox;
class AttrWidgetEnum  : public AttributeWidget
{
private:
    ComboBox *m_comboBox = nullptr;

public:
    AttrWidgetEnum(const XMLAttribute &xmlAttribute,
                   InspectorWidget *m_inspectorWidget);

    virtual void SetValue(int index);
    virtual int GetValue();

    virtual void Refresh(const XMLAttribute &attribute) override;
};

class ComboBox : public QComboBox
{
public:
    ComboBox(QWidget *parent = nullptr);

private:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
