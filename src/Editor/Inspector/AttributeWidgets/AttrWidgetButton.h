#ifndef INSPECTORBUTTONSW_H
#define INSPECTORBUTTONSW_H

#include <QPushButton>

#include "Bang/XMLAttribute.h"
#include "Bang/AttributeWidget.h"
#include "Bang/IAttrWidgetButtonListener.h"

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
