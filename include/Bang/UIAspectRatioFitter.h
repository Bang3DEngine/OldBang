#ifndef UIASPECTRATIOFITTER_H
#define UIASPECTRATIOFITTER_H

#include "Bang/Component.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/AspectRatioMode.h"
#include "Bang/ILayoutSelfController.h"

NAMESPACE_BANG_BEGIN

class UIAspectRatioFitter : public Component,
                            public ILayoutSelfController
{
    COMPONENT(UIAspectRatioFitter)

public:

    void SetAspectRatio(float aspectRatio);
    void SetAspectRatio(const Vector2 &size);
    void SetAspectRatio(const Vector2i &size);
    float GetAspectRatio() const;

    void SetAspectRatioMode(AspectRatioMode arMode);
    AspectRatioMode GetAspectRatioMode() const;

    // ILayoutSelfController
    void ApplyLayout(Axis axis) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // ITransformListener
    void OnTransformChanged() override;
    void OnParentTransformChanged() override;

    // IInvalidatable
    void OnInvalidated() override;

private:
    float m_aspectRatio = -1.0f;
    AspectRatioMode m_aspectRatioMode = Undef<AspectRatioMode>();

    UIAspectRatioFitter();
    virtual ~UIAspectRatioFitter();
};

NAMESPACE_BANG_END

#endif // UIASPECTRATIOFITTER_H

