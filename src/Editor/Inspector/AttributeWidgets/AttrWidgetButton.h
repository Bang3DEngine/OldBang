#ifndef INSPECTORBUTTONSW_H
#define INSPECTORBUTTONSW_H

#include <QPushButton>

#include "Bang/AttributeWidget.h"
#include "Bang/IAttrWidgetButtonListener.h"

class AttrWidgetButton : public AttributeWidget
{
    Q_OBJECT

private:
    QPushButton *m_button = nullptr;
    IAttrWidgetButtonListener *m_listener = nullptr;

public:
    AttrWidgetButton(const String &buttonText,
                     IAttrWidgetButtonListener *listener);

    void SetValue(const String &buttonText);
    String GetValue() const;

    virtual void Refresh() override;

    QSize sizeHint() const;

public slots:
    void OnButtonClicked();
};


#endif // INSPECTORBUTTONSW_H
