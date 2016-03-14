#ifndef INSPECTORPARTSLOTWIDGET_H
#define INSPECTORPARTSLOTWIDGET_H

#include <QWidget>

#include "InspectorPartWidget.h"

class InspectorPartSW : public QWidget
{
    Q_OBJECT

protected:
    std::string label;
    InspectorPartWidget *parent;

    InspectorPartSW(const std::string &label, InspectorPartWidget *parent);
};

#endif // INSPECTORPARTSLOTWIDGET_H
