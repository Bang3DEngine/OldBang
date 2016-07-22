#ifndef INSPECTORBUTTONSW_H
#define INSPECTORBUTTONSW_H

#include <QPushButton>
#include "InspectorSW.h"

class InspectorButtonSW : public InspectorSW
{
    Q_OBJECT

private:
    QPushButton *m_button = nullptr;
    std::function<void()> m_onClickFunction;

public:
    InspectorButtonSW(const std::string &m_label,
                      InspectorWidget *m_parent,
                      std::function<void()> m_onClickFunction);

    QSize sizeHint() const;

public slots:
    void OnButtonClicked(bool _);
};


#endif // INSPECTORBUTTONSW_H
