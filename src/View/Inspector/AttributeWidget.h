#ifndef INSPECTORSW_H
#define INSPECTORSW_H


#include "XMLAttribute.h"
#include "DragDropAgent.h"
#include "IDragDropListener.h"

class QLabel;
class XMLNode;
class QBoxLayout;
class InspectorWidget;
class AttributeWidget : public DragDropQWidget,
                        public IDragDropListener
{
    Q_OBJECT

protected:
    QBoxLayout *m_layout = nullptr;
    QLabel *m_label = nullptr;
    int m_rowIndexInGridLayout = 0;

    XMLAttribute m_xmlAttribute;

    bool m_readonly = false;
    bool m_enabled  = false;
    bool m_inlined  = false;
    bool m_hidden   = false;

    InspectorWidget *m_inspectorWidget = nullptr;
    XMLNode *m_relatedXMLInfo = nullptr;

    AttributeWidget(const XMLAttribute &xmlAttribute,
                    InspectorWidget *inspectorWidget,
                    bool isSubWidget = false,
                    bool createLabel = true,
                    bool labelAbove  = false);

    void AfterConstructor();

public:

    virtual void Refresh(const XMLAttribute &attribute);

    static AttributeWidget* FromXMLAttribute(const XMLAttribute &xmlAttribute,
                                             InspectorWidget *inspectorWidget);
};

#endif // INSPECTORSW_H
