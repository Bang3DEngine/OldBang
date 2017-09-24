#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/UIGroupLayout.h"

NAMESPACE_BANG_BEGIN

FORWARD class RectTransform;

class UIDirLayout : public UIGroupLayout
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

private:
    int m_spacingPx = 0;
    bool m_vertical = false;

    void ApplyLayoutToChildRectTransform(const Recti &layoutRect,
                                         RectTransform *childRT,
                                         const Vector2i &position,
                                         const Vector2i &childRTSize);

    void FillChildrenSizes(const Recti &layoutRect,
                           Array<Vector2i> *childrenSizes,
                           Vector2i *availableSize,
                           LayoutSizeType sizeType);
};

NAMESPACE_BANG_END

#endif // UIDIRLAYOUT_H
