#ifndef POSTPROCESSEFFECT_H
#define POSTPROCESSEFFECT_H

#include "Bang/Component.h"

FORWARD class G_Shader;
FORWARD class ShaderProgram;

class PostProcessEffect : public Component
{
    COMPONENT(PostProcessEffect)

public:
    enum Type { AfterScene = 0, AfterCanvas };

    PostProcessEffect();
    virtual ~PostProcessEffect();

    // Renderer
    virtual void OnRender(RenderPass renderPass) override;

    void SetType(Type type);
    void SetPriority(int priority);
    void SetPostProcessShader(G_Shader *postProcessShader);

    Type GetType() const;
    int GetPriority() const;
    ShaderProgram* GetPostProcessShaderProgram() const;
    G_Shader* GetPostProcessShader() const;
    Path GetPostProcessShaderFilepath() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    friend bool operator<(const PostProcessEffect& lhs,
                          const PostProcessEffect& rhs);

private:
    ShaderProgram *m_shaderProgram = nullptr;
    G_Shader *p_postProcessShader    = nullptr;
    Type m_type = Type::AfterScene;
    int m_priority = 0;
};

#endif // POSTPROCESSEFFECT_H
