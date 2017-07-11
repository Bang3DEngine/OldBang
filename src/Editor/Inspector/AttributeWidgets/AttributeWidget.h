#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QLabel>
#include <QHBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/XMLAttribute.h"
#include "Bang/DragDropQWidget.h"
#include "Bang/IDragDropListener.h"

class QLabel;
class XMLNode;
class QHBoxLayout;
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
    QHBoxLayout m_horizontalLayout;
    QLabel *m_attrNameLabel = nullptr;
    int m_rowIndexInGridLayout = 0;
    int m_heightSizeHint = -1;

    XMLAttribute m_xmlAttribute;

    bool m_readonly = false;
    bool m_enabled  = false;
    bool m_inlined  = false;
    bool m_hidden   = false;

    AttributeWidget(const XMLAttribute &xmlAttribute,
                    bool isSubWidget = false,
                    bool createLabel = true,
                    bool labelAbove  = false);

    void AfterConstructor();
};

#endif // INSPECTORSW_H
