#ifndef INSPECTORBUTTONSW_H
#define INSPECTORBUTTONSW_H

#include <QPushButton>
#include "InspectorSW.h"

class InspectorButtonSW : public InspectorSW
{
    Q_OBJECT

private:
    QPushButton *button = nullptr;
    std::function<void()> onClickFunction;

public:
    InspectorButtonSW(const std::string &label,
                      InspectorWidget *parent,
                      std::function<void()> onClickFunction);

    QSize sizeHint() const;

public slots:
    void OnButtonClicked(bool _);
};


#endif // INSPECTORBUTTONSW_H
