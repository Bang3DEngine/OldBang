#ifndef POSTPROCESSEFFECTINSPECTORWIDGET_H
#define POSTPROCESSEFFECTINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"
#include "Bang/PostProcessEffect.h"

class PostProcessEffectInspectorWidget : public ComponentWidget
{
public:
    PostProcessEffectInspectorWidget(PostProcessEffect *postProcessEffect);
    virtual ~PostProcessEffectInspectorWidget();

protected:
    virtual void InitExtra() override;
    virtual void OnUpdate() override;
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    virtual void OnDestroy() override;

    PostProcessEffect *p_postProcessEffect = nullptr;

    AttrWidgetFile                          *m_postProcessShaderAW = nullptr;
    AttrWidgetInt                           *m_priorityAW          = nullptr;
    AttrWidgetEnum<PostProcessEffect::Type> *m_typeAW              = nullptr;
};


#endif // POSTPROCESSEFFECTINSPECTORWIDGET_H
