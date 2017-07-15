#ifndef UITEXTINSPECTORWIDGET_H
#define UITEXTINSPECTORWIDGET_H

#include "Bang/UIText.h"
#include "Bang/UIRendererInspectorWidget.h"

class UITextInspectorWidget : public UIRendererInspectorWidget
{
public:
    UITextInspectorWidget(UIText *uiText);
    virtual ~UITextInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    UIText *p_uiText = nullptr;

    AttrWidgetFile                              *m_fontAW     = nullptr;
    AttrWidgetString                            *m_contentAW  = nullptr;
    AttrWidgetFloat                             *m_textSizeAW = nullptr;
    AttrWidgetFloat                             *m_hSpacingAW = nullptr;
    AttrWidgetEnum<UIText::HorizontalAlignment> *m_hAlignAW   = nullptr;
    AttrWidgetEnum<UIText::VerticalAlignment>   *m_vAlignAW   = nullptr;
    AttrWidgetBool                              *m_kerningAW  = nullptr;
};

#endif // UITEXTINSPECTORWIDGET_H
