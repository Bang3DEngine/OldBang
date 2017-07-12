#ifndef INSPECTORSW_H
#define INSPECTORSW_H

#include <QLabel>
#include <QHBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/DragDropQWidget.h"
#include "Bang/IDragDropListener.h"

class IAttributeWidget
{
};

class QLabel;
class QHBoxLayout;
class AttributeWidget : public DragDropQWidget,
                        public IAttributeWidget,
                        public IDragDropListener
{
    Q_OBJECT

public:
    AttributeWidget();
    virtual ~AttributeWidget();

    virtual void Refresh();
    virtual int GetHeightSizeHint() const;

    void SetHeightSizeHint(int heightSizeHint);
    void SetVisible(bool visible);
    bool IsVisible() const;

    void SetLabelText(const String &labelText);
    void SetLabelColor(const Color &color);

protected:
    QHBoxLayout m_horizontalLayout;

    void CreateLabel(const String &labelText);

private:
    QLabel *m_nameLabel = nullptr;
    int m_heightSizeHint = -1;

protected slots:
    void OnValueChanged();

signals:
    void OnValueChanged(IAttributeWidget *attrWidget);
};

Q_DECLARE_METATYPE(IAttributeWidget)

#endif // INSPECTORSW_H
