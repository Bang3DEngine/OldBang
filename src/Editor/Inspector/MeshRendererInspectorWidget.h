#ifndef MESHRENDERERINSPECTORWIDGET_H
#define MESHRENDERERINSPECTORWIDGET_H

#include "Bang/MeshRenderer.h"
#include "Bang/RendererInspectorWidget.h"

class MeshRendererInspectorWidget : public RendererInspectorWidget
{
public:
    MeshRendererInspectorWidget(MeshRenderer *meshRenderer);
    virtual ~MeshRendererInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    MeshRenderer *p_meshRenderer = nullptr;

    AttrWidgetFile *m_meshAW = nullptr;
};

#endif // MESHRENDERERINSPECTORWIDGET_H
