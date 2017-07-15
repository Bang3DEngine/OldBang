#ifndef FONTINSPECTORWIDGET_H
#define FONTINSPECTORWIDGET_H

#include "Bang/Font.h"
#include "Bang/FontFile.h"
#include "Bang/AssetInspectorWidget.h"

class FontInspectorWidget : public AssetInspectorWidget<Font>
{
public:
    FontInspectorWidget(const FontFile &FontFile);
    virtual ~FontInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;

    AttrWidgetFile *m_ttfFontAW = nullptr;
};


#endif // FONTINSPECTORWIDGET_H
