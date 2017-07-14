#ifndef TEXTURE2DINSPECTORWIDGET_H
#define TEXTURE2DINSPECTORWIDGET_H

#include "Bang/Texture2D.h"
#include "Bang/Texture2DFile.h"
#include "Bang/AssetInspectorWidget.h"

class Texture2DInspectorWidget : public AssetInspectorWidget<Texture2D>
{
public:
    Texture2DInspectorWidget(const Texture2DFile &texFile);
    virtual ~Texture2DInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;

    AttrWidgetFile                         *m_imageFileAW = nullptr;
    AttrWidgetEnum<G_Texture::FilterMode> *m_filterModeAW = nullptr;
    AttrWidgetFloat                      *m_alphaCutoffAW = nullptr;
};


#endif // TEXTURE2DINSPECTORWIDGET_H
