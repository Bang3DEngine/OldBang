#include "Bang/Texture2DInspectorWidget.h"

Texture2DInspectorWidget::Texture2DInspectorWidget(const Texture2DFile &texFile)
    : AssetInspectorWidget(texFile)
{
}

Texture2DInspectorWidget::~Texture2DInspectorWidget()
{

}
void Texture2DInspectorWidget::OnDestroy()
{
    AssetInspectorWidget::OnDestroy();
}

void Texture2DInspectorWidget::InitExtra()
{
    AssetInspectorWidget::InitExtra();

    m_imageFileAW   = new AttrWidgetFile("Image");
    m_filterModeAW  = new AttrWidgetEnum<G_Texture::FilterMode>(
                             "Filter Mode",
                             {G_Texture::FilterMode::Nearest,
                              G_Texture::FilterMode::Linear,
                              G_Texture::FilterMode::Trilinear},
                              {"Nearest", "Linear", "Trilinear"});
    m_alphaCutoffAW = new AttrWidgetFloat("Alpha Cutoff");

    InsertAttributeWidget(m_imageFileAW);
    InsertAttributeWidget(m_filterModeAW);
    InsertAttributeWidget(m_alphaCutoffAW);
}

void Texture2DInspectorWidget::Refresh()
{
    AssetInspectorWidget::Refresh();

    Texture2D *tex = GetAsset();
    m_imageFileAW->SetValue(tex->GetImageFilepath());
    m_filterModeAW->SetValue(tex->GetFilterMode());
    m_alphaCutoffAW->SetValue(tex->GetAlphaCutoff());
}

void Texture2DInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    AssetInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    Texture2D *tex = GetAsset();
    if (attrWidget == m_imageFileAW)
    {
        tex->LoadFromImage(m_imageFileAW->GetPath());
    }
    else if (attrWidget == m_filterModeAW)
    {
        tex->SetFilterMode(m_filterModeAW->GetValue());
    }
    else if (attrWidget == m_alphaCutoffAW)
    {
        tex->SetAlphaCutoff(m_alphaCutoffAW->GetValue());
    }

    RewriteAsset();
}
