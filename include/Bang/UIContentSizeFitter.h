#ifndef UICONTENTSIZEFITTER_H
#define UICONTENTSIZEFITTER_H

#include "Bang/Component.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/ILayoutSelfController.h"

NAMESPACE_BANG_BEGIN

class UIContentSizeFitter : public Component,
                            public ILayoutSelfController
{
    COMPONENT(UIContentSizeFitter)

public:
    void SetHorizontalSizeType(LayoutSizeType sizeType);
    void SetVerticalSizeType(LayoutSizeType sizeType);

    LayoutSizeType GetHorizontalSizeType() const;
    LayoutSizeType GetVerticalSizeType() const;

    // ILayoutElement
    virtual void ApplyLayout(Axis axis) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // IInvalidatable
    void OnInvalidated() override;

private:
    LayoutSizeType m_verticalSizeType   = LayoutSizeType::None;
    LayoutSizeType m_horizontalSizeType = LayoutSizeType::None;

    UIContentSizeFitter();
    virtual ~UIContentSizeFitter();
};

NAMESPACE_BANG_END

#endif // UICONTENTSIZEFITTER_H
