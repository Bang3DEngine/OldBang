#ifndef INSPECTORBUTTONSW_H
#define INSPECTORBUTTONSW_H

#include <QPushButton>

#include "XMLNode.h"
#include "AttributeWidget.h"

class AttrWidgetButton : public AttributeWidget
{
    Q_OBJECT

private:
    QPushButton *m_button = nullptr;
    std::function<void()> m_onClickFunction;

public:
    AttrWidgetButton(const std::string &m_label,
                     std::function<void()> m_onClickFunction,
                     InspectorWidget *m_parent);

    QSize sizeHint() const;

public slots:
    void OnButtonClicked(bool _);
};


#endif // INSPECTORBUTTONSW_H
