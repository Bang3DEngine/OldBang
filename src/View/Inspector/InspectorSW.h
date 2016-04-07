#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QWidget>

#include "InspectorPartWidget.h"

class InspectorSW : public QWidget
{
    Q_OBJECT

protected:
    std::string label = "";
    InspectorWidget *parent = nullptr;

    InspectorSW(const std::string &label, InspectorWidget *parent);
};

#endif // INSPECTORSW_H
