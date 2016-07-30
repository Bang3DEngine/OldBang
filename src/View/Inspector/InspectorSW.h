#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QWidget>

#include "InspectorComponentWidget.h"

/*
*/
class InspectorSW : public QWidget
{
    Q_OBJECT

protected:
    std::string m_label = "";
    InspectorWidget *m_parent = nullptr;
    XMLNode *m_relatedXMLInfo = nullptr;

    InspectorSW(const std::string &label,
                InspectorWidget *parent,
                XMLNode *relatedXMLInfo);
};

#endif // INSPECTORSW_H
