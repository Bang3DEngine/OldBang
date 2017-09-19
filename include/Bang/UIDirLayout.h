#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Component.h"
#include "Bang/ILayoutElement.h"
#include "Bang/ILayoutController.h"

NAMESPACE_BANG_BEGIN

class UIDirLayout : public Component,
                    public ILayoutElement,
                    public ILayoutController
{
    COMPONENT(UIDirLayout)

public:

    // ILayoutController
    virtual void ApplyLayout() override;

    // ILayoutElement
    virtual Vector2i CalculateTotalMinSize() const override;
    virtual Vector2i CalculateTotalPreferredSize() const override;
    virtual Vector2i CalculateTotalFlexiblePxSize() const override;
    Vector2i CalculateTotalSize(LayoutSizeType sizeType) const;

    void SetSpacing(int spacingPx);

    int GetSpacing() const;
    Vector2i GetDir() const;
    Vector2i GetTotalSpacing() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    UIDirLayout();
    UIDirLayout(bool vertical);
    virtual ~UIDirLayout();

    void FillChildrenSizes(Array<Vector2i> *childrenSizes,
                           Vector2i *availableSize,
                           LayoutSizeType sizeType);

private:
    int m_spacingPx = 0;
    bool m_vertical = false;
};

NAMESPACE_BANG_END

#endif // UIDIRLAYOUT_H
