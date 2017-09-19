#ifndef UICONTENTSIZEFITTER_H
#define UICONTENTSIZEFITTER_H

#include "Bang/Component.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/ILayoutController.h"

NAMESPACE_BANG_BEGIN

class UIContentSizeFitter : public Component,
                            public ILayoutController
{
public:
    UIContentSizeFitter();
    virtual ~UIContentSizeFitter();

    void SetHorizontalSizeType(LayoutSizeType sizeType);
    LayoutSizeType GetHorizontalSizeType() const;

    void SetVerticalSizeType(LayoutSizeType sizeType);
    LayoutSizeType GetVerticalSizeType() const;

    // ILayoutElement
    virtual void ApplyLayout() override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    LayoutSizeType m_verticalSizeType   = LayoutSizeType::None;
    LayoutSizeType m_horizontalSizeType = LayoutSizeType::None;
};

NAMESPACE_BANG_END

#endif // UICONTENTSIZEFITTER_H
