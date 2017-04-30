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
    enum Type
    {
        AfterScene = 0,
        AfterCanvas
    };

    PostProcessEffect();
    virtual ~PostProcessEffect();

    void SetType(Type type);
    void SetPriority(int priority);
    void SetPostProcessShader(Shader *postProcessShader);

    virtual void CloneInto(ICloneable *clone) const override;

    Type GetType() const;
    int GetPriority() const;
    ShaderProgram* GetPostProcessShaderProgram() const;
    Shader* GetPostProcessShader() const;
    Path GetPostProcessShaderFilepath() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    bool operator<(const PostProcessEffect& ppe) const;

private:
    ShaderProgram *m_shaderProgram = nullptr;
    Shader *p_postProcessShader    = nullptr;
    Type m_type = Type::AfterScene;
    int m_priority = 0;
};

#endif // POSTPROCESSEFFECT_H
