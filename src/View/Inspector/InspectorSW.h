#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QWidget>

#include "InspectorComponentWidget.h"

class InspectorSW : public QWidget
{
    Q_OBJECT

protected:
    std::string m_label = "";
    InspectorWidgetInfo *m_relatedInfo = nullptr;
    InspectorWidget *m_parent = nullptr;

    InspectorSW(const std::string &label,
                InspectorWidget *parent,
                InspectorWidgetInfo *relatedInfo);
};

#endif // INSPECTORSW_H
