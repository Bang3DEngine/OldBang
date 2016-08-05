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
    AttrWidgetButton(const std::string &label,
                     IAttrWidgetButtonListener *listener,
                     InspectorWidget *parent);

    void SetValue(const std::string &buttonText);
    std::string GetValue() const;

    QSize sizeHint() const;

public slots:
    void OnButtonClicked(bool _);
};


#endif // INSPECTORBUTTONSW_H
