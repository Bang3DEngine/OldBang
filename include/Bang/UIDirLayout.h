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

    Axis GetAxis() const;
    Vector2i GetDir() const;

protected:
    UIDirLayout();
    UIDirLayout(Axis axis);
    virtual ~UIDirLayout();

private:
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
    void ApplyStretches(const Vector2i &layoutRectSize,
                        Array<Vector2i> *childrenRTSizes);
};

NAMESPACE_BANG_END

#endif // UIDIRLAYOUT_H
