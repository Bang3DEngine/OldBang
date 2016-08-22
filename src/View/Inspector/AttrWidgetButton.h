#ifndef INSPECTORBUTTONSW_H
#define INSPECTORBUTTONSW_H

#include <QPushButton>

#include "XMLNode.h"
#include "AttributeWidget.h"
#include "IAttrWidgetButtonListener.h"

class AttrWidgetButton : public AttributeWidget
{
    Q_OBJECT

private:
    QPushButton *m_button = nullptr;
    IAttrWidgetButtonListener *m_listener = nullptr;

public:
    AttrWidgetButton(const XMLAttribute &xmlAttribute,
                     InspectorWidget *inspectorWidget);

    void SetValue(const String &buttonText);
    String GetValue() const;

    virtual void Refresh(const XMLAttribute &attribute) override;

    QSize sizeHint() const;

public slots:
    void OnButtonClicked(bool _);
};


#endif // INSPECTORBUTTONSW_H
