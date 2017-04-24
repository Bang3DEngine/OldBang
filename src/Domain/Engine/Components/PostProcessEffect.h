#ifndef POSTPROCESSEFFECT_H
#define POSTPROCESSEFFECT_H

#include "Bang/Component.h"

class Shader;
class ShaderProgram;
class PostProcessEffect : public Component
{
    OBJECT(PostProcessEffect)
    COMPONENT_ICON(PostProcessEffect, "Icons/ImageIcon.png")

public:
    PostProcessEffect();
    virtual ~PostProcessEffect();

    void SetPostProcessShader(Shader *postProcessShader);

    ShaderProgram* GetPostProcessShaderProgram() const;
    Shader* GetPostProcessShader() const;
    String GetPostProcessShaderFilepath() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    ShaderProgram *m_shaderProgram = nullptr;
    Shader *p_postProcessShader    = nullptr;
};

#endif // POSTPROCESSEFFECT_H
