#ifndef INSPECTORPARTSLOTWIDGET_H
#define INSPECTORPARTSLOTWIDGET_H

#include <QWidget>

#include "InspectorPartWidget.h"

class InspectorPartSW : public QWidget
{
    Q_OBJECT

protected:
    InspectorPartWidget *parent;

    InspectorPartSW(InspectorPartWidget *parent);
/*
    template <class T>
    void SetValue(const T &v) {}

    template <class T>
    T GetValue() { return 0; }
*/
};

#endif // INSPECTORPARTSLOTWIDGET_H
