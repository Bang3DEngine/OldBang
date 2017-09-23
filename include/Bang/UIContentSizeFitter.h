#ifndef UICONTENTSIZEFITTER_H
#define UICONTENTSIZEFITTER_H

#include "Bang/Component.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/ILayoutController.h"

NAMESPACE_BANG_BEGIN

enum class SizeFitMode { Sum, Max };

class UIContentSizeFitter : public Component,
                            public ILayoutController
{
public:
    UIContentSizeFitter();
    virtual ~UIContentSizeFitter();

    void SetHorizontalSizeFitMode(SizeFitMode fitMode);
    void SetVerticalSizeFitMode(SizeFitMode fitMode);
    void SetHorizontalSizeType(LayoutSizeType sizeType);
    void SetVerticalSizeType(LayoutSizeType sizeType);

    SizeFitMode GetHorizontalSizeFitMode() const;
    SizeFitMode GetVerticalSizeFitMode() const;
    LayoutSizeType GetHorizontalSizeType() const;
    LayoutSizeType GetVerticalSizeType() const;

    // ILayoutElement
    virtual void ApplyLayout() override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    SizeFitMode m_verticalFitMode       = SizeFitMode::Sum;
    SizeFitMode m_horizontalFitMode     = SizeFitMode::Sum;
    LayoutSizeType m_verticalSizeType   = LayoutSizeType::None;
    LayoutSizeType m_horizontalSizeType = LayoutSizeType::None;
};

NAMESPACE_BANG_END

#endif // UICONTENTSIZEFITTER_H
