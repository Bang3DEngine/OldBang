#ifndef INSPECTORCOMPONENTENUMSLOTWIDGET_H
#define INSPECTORCOMPONENTENUMSLOTWIDGET_H

#include <QComboBox>

#include "Bang/InspectorWidget.h"
#include "Bang/AttributeWidget.h"

class ComboBox;
class AttrWidgetEnum  : public AttributeWidget
{
private:
    ComboBox *m_comboBox = nullptr;

public:
    AttrWidgetEnum(const String &labelText);

    virtual void SetValue(int index);
    virtual int GetValue() const;

    virtual void Refresh() override;
};

class ComboBox : public QComboBox
{
public:
    ComboBox(QWidget *parent = nullptr);

private:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // INSPECTORCOMPONENTENUMSLOTWIDGET_H
