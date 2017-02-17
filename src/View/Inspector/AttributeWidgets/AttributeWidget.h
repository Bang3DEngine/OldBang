#ifndef INSPECTORSW_H
#define INSPECTORSW_H


#include "XMLAttribute.h"
#include "DragDropQWidget.h"
#include "IDragDropListener.h"

class QLabel;
class XMLNode;
class QBoxLayout;
class InspectorWidget;
class AttributeWidget : public DragDropQWidget,
                        public IDragDropListener
{
    Q_OBJECT

public:

    virtual void Refresh(const XMLAttribute &attribute);
    virtual int GetHeightSizeHint();

    static AttributeWidget* FromXMLAttribute(const XMLAttribute &xmlAttribute,
                                             InspectorWidget *inspectorWidget);

protected:
    QBoxLayout *m_layout = nullptr;
    QLabel *m_label = nullptr;
    int m_rowIndexInGridLayout = 0;
    int m_heightSizeHint = -1;

    XMLAttribute m_xmlAttribute;

    bool m_readonly = false;
    bool m_enabled  = false;
    bool m_inlined  = false;
    bool m_hidden   = false;

    InspectorWidget *m_inspectorWidget = nullptr;

    AttributeWidget(const XMLAttribute &xmlAttribute,
                    InspectorWidget *inspectorWidget,
                    bool isSubWidget = false,
                    bool createLabel = true,
                    bool labelAbove  = false);

    void AfterConstructor();
};

#endif // INSPECTORSW_H
