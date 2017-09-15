#ifndef UICONTENTSIZEFITTER_H
#define UICONTENTSIZEFITTER_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

enum SizeFit { None, Min, Preferred, Both };

class UIContentSizeFitter : public Component
{
public:
    UIContentSizeFitter();
    virtual ~UIContentSizeFitter();

    // Component
    virtual void OnUpdate() override;
    virtual void OnChildLayoutChanged() override;

    void SetHorizontalSizeFit(SizeFit sizeFit);
    SizeFit GetHorizontalSizeFit() const;

    void SetVerticalSizeFit(SizeFit sizeFit);
    SizeFit GetVerticalSizeFit() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    SizeFit m_verticalSizeFit   = SizeFit::Both;
    SizeFit m_horizontalSizeFit = SizeFit::Both;
};

NAMESPACE_BANG_END

#endif // UICONTENTSIZEFITTER_H
