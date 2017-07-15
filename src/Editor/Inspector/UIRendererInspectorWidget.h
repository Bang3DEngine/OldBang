#ifndef UIRENDERERINSPECTORWIDGET_H
#define UIRENDERERINSPECTORWIDGET_H

#include "Bang/UIRenderer.h"
#include "Bang/RendererInspectorWidget.h"

class UIRendererInspectorWidget : public RendererInspectorWidget
{
public:
    UIRendererInspectorWidget(UIRenderer *uiRenderer);
    virtual ~UIRendererInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

    AttrWidgetColor *m_tintAW = nullptr;

private:
    UIRenderer *p_uiRenderer = nullptr;
};

#endif // UIRENDERERINSPECTORWIDGET_H
