#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QWidget>

#include "InspectorComponentWidget.h"

class InspectorSW : public QWidget
{
    Q_OBJECT

protected:
    std::string m_label = "";
    InspectorWidget *p_parent = nullptr;

    InspectorSW(const std::string &label, InspectorWidget *parent);
};

#endif // INSPECTORSW_H
