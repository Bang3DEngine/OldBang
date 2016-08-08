#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QWidget>

#include "ComponentWidget.h"
#include "IDroppableWidget.h"

class AttributeWidget : public IDroppableQWidget
{
    Q_OBJECT

private:
    void AddLabelWidget(const XMLAttribute &xmlAttribute,
                        QBoxLayout *layout);

protected:
    QBoxLayout *m_layout = nullptr;

    bool m_readonly = false;
    bool m_enabled  = false;
    bool m_inlined  = false;
    bool m_hidden   = false;

    InspectorWidget *m_inspectorWidget = nullptr;
    XMLNode *m_relatedXMLInfo = nullptr;

    AttributeWidget(const XMLAttribute &xmlAttribute,
                    InspectorWidget *inspectorWidget,
                    bool createLabel = true);

    void AfterConstructor();

public:

    virtual void Refresh(const XMLAttribute &attribute);

    static AttributeWidget* FromXMLAttribute(const XMLAttribute &xmlAttribute,
                                             InspectorWidget *inspectorWidget);
};

#endif // INSPECTORSW_H
