#include "Bang/FontInspectorWidget.h"

FontInspectorWidget::FontInspectorWidget(
        const FontFile &FontFile)
    : AssetInspectorWidget(FontFile)
{
}

FontInspectorWidget::~FontInspectorWidget()
{

}
void FontInspectorWidget::OnDestroy()
{
    AssetInspectorWidget::OnDestroy();
}

void FontInspectorWidget::InitExtra()
{
    AssetInspectorWidget::InitExtra();
    m_ttfFontAW = new AttrWidgetFile("TTF File");

    InsertAttributeWidget(m_ttfFontAW);
}

void FontInspectorWidget::OnUpdate()
{
    AssetInspectorWidget::OnUpdate();

    Font *font = GetAsset();
    if (font)
    {
        m_ttfFontAW->SetValue( font->GetTTFFilepath() );
    }
}

void FontInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    AssetInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    Font *font = GetAsset();
    if (attrWidget == m_ttfFontAW)
    {
        font->LoadFromTTF(m_ttfFontAW->GetPath());
    }

    emit Changed(this);
}
