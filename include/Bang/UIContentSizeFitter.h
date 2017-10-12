#ifndef UICONTENTSIZEFITTER_H
#define UICONTENTSIZEFITTER_H

#include "Bang/Component.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/ILayoutSelfController.h"

NAMESPACE_BANG_BEGIN

class UIContentSizeFitter : public Component,
                            public ILayoutSelfController
{
public:
    UIContentSizeFitter();
    virtual ~UIContentSizeFitter();

    void SetHorizontalSizeType(LayoutSizeType sizeType);
    void SetVerticalSizeType(LayoutSizeType sizeType);

    LayoutSizeType GetHorizontalSizeType() const;
    LayoutSizeType GetVerticalSizeType() const;

    // ILayoutElement
    virtual void _ApplyLayout() override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    LayoutSizeType m_verticalSizeType   = LayoutSizeType::None;
    LayoutSizeType m_horizontalSizeType = LayoutSizeType::None;
};

NAMESPACE_BANG_END

#endif // UICONTENTSIZEFITTER_H
