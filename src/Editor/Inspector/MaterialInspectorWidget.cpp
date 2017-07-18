#include "Bang/MaterialInspectorWidget.h"

#include "Bang/G_Shader.h"
#include "Bang/Texture2D.h"
#include "Bang/Extensions.h"
#include "Bang/IconManager.h"
#include "Bang/ShaderProgram.h"

MaterialInspectorWidget::MaterialInspectorWidget(
        const MaterialFile &materialFile)
    : AssetInspectorWidget(materialFile)
{
}

MaterialInspectorWidget::~MaterialInspectorWidget()
{

}
void MaterialInspectorWidget::OnDestroy()
{
    AssetInspectorWidget::OnDestroy();
}

void MaterialInspectorWidget::InitExtra()
{
    AssetInspectorWidget::InitExtra();

    m_diffuseColorAW     = new AttrWidgetColor("Diffuse Color");
    m_shininessAW        = new AttrWidgetFloat("Shininess");
    m_receivesLightingAW = new AttrWidgetBool("Receives Lighting");
    m_uvMultiplyAW       = new AttrWidgetVectorFloat("Uv Multiply", 2);
    m_textureAW          = new AttrWidgetFile("Texture",
                                              {Extensions::Get<Texture2D>()});
    m_vShaderAW          = new AttrWidgetFile("Vertex Shader",
                                              Extensions::GetVertexShaderList());
    m_fShaderAW          = new AttrWidgetFile("Fragment Shader",
                                              Extensions::GetFragmentShaderList());

    InsertAttributeWidget(m_diffuseColorAW);
    InsertAttributeWidget(m_shininessAW);
    InsertAttributeWidget(m_receivesLightingAW);
    InsertAttributeWidget(m_uvMultiplyAW);
    InsertAttributeWidget(m_textureAW);
    InsertAttributeWidget(m_vShaderAW);
    InsertAttributeWidget(m_fShaderAW);
}

void MaterialInspectorWidget::OnUpdate()
{
    AssetInspectorWidget::OnUpdate();

    Material *mat = GetAsset();

    m_diffuseColorAW->SetValue(mat->GetDiffuseColor());
    m_shininessAW->SetValue(mat->GetShininess());
    m_receivesLightingAW->SetValue(mat->ReceivesLighting());
    m_uvMultiplyAW->SetValue(mat->GetUvMultiply());

    const Texture2D *tex = mat->GetTexture();
    if (tex) { m_textureAW->SetValue(tex->GetFilepath()); }

    ShaderProgram *sp = mat->GetShaderProgram();
    if (sp)
    {
        G_Shader *vShader = sp->GetVertexShader();
        G_Shader *fShader = sp->GetFragmentShader();
        m_vShaderAW->SetValue(vShader ? vShader->GetFilepath() : Path::Empty);
        m_fShaderAW->SetValue(fShader ? fShader->GetFilepath() : Path::Empty);
    }
}

void MaterialInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    AssetInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    Material *mat = GetAsset();
    if (attrWidget == m_diffuseColorAW)
    {
        mat->SetDiffuseColor(m_diffuseColorAW->GetValue());
    }
    else if (attrWidget == m_shininessAW)
    {
        mat->SetShininess(m_shininessAW->GetValue());
    }
    else if (attrWidget == m_receivesLightingAW)
    {
        mat->SetReceivesLighting(m_receivesLightingAW->GetValue());
    }
    else if (attrWidget == m_uvMultiplyAW)
    {
        mat->SetUvMultiply(m_uvMultiplyAW->GetVector2());
    }
    else if (attrWidget == m_textureAW)
    {
        Texture2D *tex = AssetsManager::Load<Texture2D>(m_textureAW->GetPath());
        mat->SetTexture(tex);
    }
    else
    {
        ShaderProgram *sp = mat->GetShaderProgram();
        if (sp && (attrWidget == m_vShaderAW || attrWidget == m_fShaderAW) )
        {
            ShaderProgram *sp = new ShaderProgram();
            sp->Load(m_vShaderAW->GetPath(), m_fShaderAW->GetPath());
            mat->SetShaderProgram(sp);
        }
    }

    IconManager::InvalidatePixmap( m_file.GetPath() );
    emit Changed(this);
}
