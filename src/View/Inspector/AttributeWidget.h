#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QWidget>

#include "ComponentWidget.h"

class AttributeWidget : public QWidget
{
    Q_OBJECT

protected:
    std::string m_label = "";
    InspectorWidget *m_parent = nullptr;
    XMLNode *m_relatedXMLInfo = nullptr;

    AttributeWidget(const std::string &label,
                    InspectorWidget *parent);

    /**
     * @brief GetLabelWidget
     * @param label
     * @return A label that is properly formatted. Add this
     * to the layout, where you expect the attributewidget label to be.
     */
    QLabel* GetLabelWidget(const std::string &label);

public:
    static AttributeWidget* FromXMLAttribute(const XMLAttribute &xmlAttribute,
                                             InspectorWidget *inspectorWidget);
};

#endif // INSPECTORSW_H
