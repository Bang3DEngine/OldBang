#ifndef RENDERERINSPECTORWIDGET_H
#define RENDERERINSPECTORWIDGET_H

#include "Bang/Renderer.h"
#include "Bang/ComponentWidget.h"

class RendererInspectorWidget : public ComponentWidget
{
public:
    RendererInspectorWidget(Renderer *renderer);
    virtual ~RendererInspectorWidget();

protected:
    virtual void InitExtra() override;
    virtual void Refresh() override;
    virtual void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    virtual void OnDestroy() override;

    Renderer *p_renderer = nullptr;

    AttrWidgetFile  *m_materialAW      = nullptr;
    AttrWidgetBool  *m_drawWireframeAW = nullptr;
    AttrWidgetFloat *m_lineWidthAW     = nullptr;
};

#endif // RENDERERINSPECTORWIDGET_H
