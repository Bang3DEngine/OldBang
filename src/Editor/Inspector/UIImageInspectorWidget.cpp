#include "Bang/UIImageInspectorWidget.h"

#include "Bang/Texture2D.h"
#include "Bang/AssetsManager.h"

UIImageInspectorWidget::UIImageInspectorWidget(UIImage *uiImage)
    : UIRendererInspectorWidget(uiImage)
{
    p_uiImage = uiImage;
}

UIImageInspectorWidget::~UIImageInspectorWidget()
{
}

void UIImageInspectorWidget::InitExtra()
{
    UIRendererInspectorWidget::InitExtra();

    m_imageAW = new AttrWidgetFile("Image");

    InsertAttributeWidget(m_imageAW);
}

void UIImageInspectorWidget::OnUpdate()
{
    UIRendererInspectorWidget::OnUpdate();

    Texture2D *tex = p_uiImage->GetImageTexture();
    Path texPath = tex ? tex->GetFilepath() : Path::Empty;
    m_imageAW->SetValue(texPath);
}

void UIImageInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    UIRendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_imageAW)
    {
        Texture2D *tex = AssetsManager::Load<Texture2D>(m_imageAW->GetPath());
        p_uiImage->SetImage(tex);
    }

    emit Changed(this);
}

void UIImageInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
