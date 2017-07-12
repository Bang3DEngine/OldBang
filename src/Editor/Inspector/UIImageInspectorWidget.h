#ifndef UIIMAGEINSPECTORWIDGET_H
#define UIIMAGEINSPECTORWIDGET_H

#include "Bang/UIImage.h"
#include "Bang/UIRendererInspectorWidget.h"

class UIImageInspectorWidget : public UIRendererInspectorWidget
{
public:
    UIImageInspectorWidget(UIImage *uiImage);
    virtual ~UIImageInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    UIImage *p_uiImage = nullptr;

    AttrWidgetFile *m_imageAW   = nullptr;
};

#endif // UIIMAGEINSPECTORWIDGET_H
