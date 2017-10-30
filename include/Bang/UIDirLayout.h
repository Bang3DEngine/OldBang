#ifndef UIDIRLAYOUT_H
#define UIDIRLAYOUT_H

#include "Bang/Axis.h"
#include "Bang/List.h"
#include "Bang/UIGroupLayout.h"

NAMESPACE_BANG_BEGIN

FORWARD class RectTransform;

class UIDirLayout : public UIGroupLayout
{
    COMPONENT(UIDirLayout)

public:
    // ILayoutController
    virtual void ApplyLayout(Axis axis) override;

    // ILayoutElement
    virtual void CalculateLayout(Axis axis) override;

    void SetSpacing(int spacingPx);

    int GetSpacing() const;
    Axis GetAxis() const;
    Vector2i GetDir() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    UIDirLayout();
    UIDirLayout(Axis axis);
    virtual ~UIDirLayout();

private:
    int m_spacingPx = 0;
    Axis m_axis = Axis::Horizontal;

    Vector2i GetTotalSpacing(const List<GameObject*> &children) const;

    void ApplyLayoutToChildRectTransform(Axis rebuildPassAxis,
                                         const Vector2i &layoutRectSize,
                                         RectTransform *childRT,
                                         const Vector2i &position,
                                         const Vector2i &childRTSize);

    void FillChildrenMinSizes(const Vector2i &layoutRectSize,
                              const List<GameObject*> &children,
                              Array<Vector2i> *childrenRTSizes,
                              Vector2i *availableSpace);
    void FillChildrenPreferredSizes(const Vector2i &layoutRectSize,
                                    const List<GameObject*> &children,
                                    Array<Vector2i> *childrenRTSizes,
                                    Vector2i *availableSpace);
    void FillChildrenFlexibleSizes(const Vector2i &layoutRectSize,
                                   const List<GameObject*> &children,
                                   Array<Vector2i> *childrenRTSizes,
                                   Vector2i *availableSpace);
};

NAMESPACE_BANG_END

#endif // UIDIRLAYOUT_H
