#ifndef POSTPROCESSEFFECT_H
#define POSTPROCESSEFFECT_H

#include "Bang/Component.h"

class G_Shader;
class G_ShaderProgram;
class PostProcessEffect : public Component
{
    OBJECT(PostProcessEffect)

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
    void SetPostProcessShader(G_Shader *postProcessShader);

    virtual void CloneInto(ICloneable *clone) const override;

    Type GetType() const;
    int GetPriority() const;
    G_ShaderProgram* GetPostProcessShaderProgram() const;
    G_Shader* GetPostProcessShader() const;
    Path GetPostProcessShaderFilepath() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    friend bool operator<(const PostProcessEffect& lhs,
                          const PostProcessEffect& rhs);

private:
    G_ShaderProgram *m_shaderProgram = nullptr;
    G_Shader *p_postProcessShader    = nullptr;
    Type m_type = Type::AfterScene;
    int m_priority = 0;
};

#endif // POSTPROCESSEFFECT_H
