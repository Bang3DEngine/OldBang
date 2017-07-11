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
class AttributeWidget : public DragDropQWidget,
                        public IDragDropListener
{
    Q_OBJECT

public:

    virtual void Refresh(const XMLAttribute &attribute);
    virtual int GetHeightSizeHint() const;

    void SetHeightSizeHint(int heightSizeHint);
    void SetVisible(bool visible);
    bool IsVisible() const;

    static AttributeWidget* FromXMLAttribute(const XMLAttribute &xmlAttribute);

protected:
    QHBoxLayout m_horizontalLayout;
    bool m_readonly = false;

    AttributeWidget(const XMLAttribute &xmlAttribute,
                    bool isSubWidget = false,
                    bool createLabel = true,
                    bool labelAbove  = false);

    void AfterConstructor();

private:
    int m_heightSizeHint = -1;

signals:
    void OnValueChanged();
};

#endif // INSPECTORSW_H
