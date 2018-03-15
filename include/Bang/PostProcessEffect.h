#ifndef POSTPROCESSEFFECT_H
#define POSTPROCESSEFFECT_H

#include "Bang/Component.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD class Shader;
FORWARD class ShaderProgram;

class PostProcessEffect : public Component
{
    COMPONENT(PostProcessEffect)

public:
    enum class Type { AfterScene = 0, AfterCanvas };

    PostProcessEffect();
    virtual ~PostProcessEffect();

    // Component
    virtual void OnRender(RenderPass renderPass) override;

    void SetType(Type type);
    void SetPriority(int priority);
    void SetPostProcessShader(Shader* postProcessShader);

    Type GetType() const;
    int GetPriority() const;
    ShaderProgram* GetPostProcessShaderProgram() const;
    Shader* GetPostProcessShader() const;
    Path GetPostProcessShaderFilepath() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    friend bool operator<(const PostProcessEffect& lhs,
                          const PostProcessEffect& rhs);

protected:
    bool MustBeRendered(RenderPass renderPass) const;

private:
    RH<ShaderProgram> p_shaderProgram;
    RH<Shader> p_postProcessShader;
    Type m_type = Type::AfterScene;
    int m_priority = 0;
};

NAMESPACE_BANG_END

#endif // POSTPROCESSEFFECT_H
