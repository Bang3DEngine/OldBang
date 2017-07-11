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
    AttrWidgetEnum(const XMLAttribute &xmlAttribute);

    virtual void SetValue(int index);
    virtual int GetValue() const;

    virtual void Refresh(const XMLAttribute &attribute) override;
    virtual XMLAttribute GetXMLAttribute() const override;
};

class ComboBox : public QComboBox
{
public:
    ComboBox(QWidget *parent = nullptr);

private:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
