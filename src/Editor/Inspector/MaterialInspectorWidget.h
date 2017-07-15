#ifndef MATERIALINSPECTORWIDGET_H
#define MATERIALINSPECTORWIDGET_H

#include "Bang/Material.h"
#include "Bang/MaterialFile.h"
#include "Bang/AssetInspectorWidget.h"

class MaterialInspectorWidget : public AssetInspectorWidget<Material>
{
public:
    MaterialInspectorWidget(const MaterialFile &materialFile);
    virtual ~MaterialInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnDestroy() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;

    AttrWidgetColor       *m_diffuseColorAW     = nullptr;
    AttrWidgetFloat       *m_shininessAW        = nullptr;
    AttrWidgetBool        *m_receivesLightingAW = nullptr;
    AttrWidgetVectorFloat *m_uvMultiplyAW       = nullptr;
    AttrWidgetFile        *m_textureAW          = nullptr;
    AttrWidgetFile        *m_vShaderAW          = nullptr;
    AttrWidgetFile        *m_fShaderAW          = nullptr;
};


#endif // MATERIALINSPECTORWIDGET_H
