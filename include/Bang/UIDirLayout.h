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
    virtual Vector2i _GetMinSize()       const override;
    virtual Vector2i _GetPreferredSize() const override;

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

    Vector2i GetSize(LayoutSizeType sizeType) const;

    void ApplyLayoutToChildRectTransform(const Vector2i &layoutRectSize,
                                         RectTransform *childRT,
                                         const Vector2i &position,
                                         const Vector2i &childRTSize);

    void FillChildrenMinSizes(const Vector2i &layoutRectSize,
                              Array<Vector2i> *childrenRTSizes,
                              Vector2i *availableSpace);
    void FillChildrenPreferredSizes(const Vector2i &layoutRectSize,
                                    Array<Vector2i> *childrenRTSizes,
                                    Vector2i *availableSpace);
    void FillChildrenFlexibleSizes(const Vector2i &layoutRectSize,
                                   Array<Vector2i> *childrenRTSizes,
                                   Vector2i *availableSpace);
};

NAMESPACE_BANG_END

#endif // UIDIRLAYOUT_H
