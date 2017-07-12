#include "Bang/PostProcessEffectInspectorWidget.h"

#include "Bang/ShaderManager.h"

PostProcessEffectInspectorWidget::PostProcessEffectInspectorWidget(
        PostProcessEffect *postProcessEffect)
    : ComponentWidget(postProcessEffect)
{
    p_postProcessEffect = postProcessEffect;
}

PostProcessEffectInspectorWidget::~PostProcessEffectInspectorWidget()
{
}

void PostProcessEffectInspectorWidget::InitExtra()
{
    ComponentWidget::InitExtra();

    m_postProcessShaderAW = new AttrWidgetFile("Post Process Shader");
    m_priorityAW          = new AttrWidgetInt("Priority");
    m_typeAW              = new AttrWidgetEnum<PostProcessEffect::Type>(
                                "Type",
                                {PostProcessEffect::Type::AfterScene,
                                 PostProcessEffect::Type::AfterCanvas},
                                {"After Scene", "After Canvas"});

    InsertAttributeWidget(m_postProcessShaderAW);
    InsertAttributeWidget(m_priorityAW);
    InsertAttributeWidget(m_typeAW);
}

void PostProcessEffectInspectorWidget::Refresh()
{
    ComponentWidget::Refresh();

    m_postProcessShaderAW->SetValue(
                p_postProcessEffect->GetPostProcessShaderFilepath() );
    m_priorityAW->SetValue( p_postProcessEffect->GetPriority() );
    m_typeAW->SetValue( p_postProcessEffect->GetType() );
}

void PostProcessEffectInspectorWidget::OnAttrWidgetValueChanged(IAttributeWidget *attrWidget)
{
    if (attrWidget == m_postProcessShaderAW)
    {
        G_Shader *postProcessShader =
                ShaderManager::Load(G_Shader::Type::Fragment,
                                    m_postProcessShaderAW->GetPath());
        p_postProcessEffect->SetPostProcessShader(postProcessShader);
    }
    else if (attrWidget == m_priorityAW)
    {
        p_postProcessEffect->SetPriority( m_priorityAW->GetValue() );
    }
    else if (attrWidget == m_typeAW)
    {
        p_postProcessEffect->SetType( m_typeAW->GetValue() );
    }
}

void PostProcessEffectInspectorWidget::OnDestroy()
{
    ComponentWidget::OnDestroy();
}
