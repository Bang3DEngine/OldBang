#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang/Asset.h"
#include "Bang/IToString.h"
#include "Bang/G_ShaderProgram.h"

class ShaderProgram : public G_ShaderProgram,
                      public Asset
{
    OBJECT(ShaderProgram)

public:
    enum Type
    {
        GBuffer,
        PostProcess,
        Other
    };

    ShaderProgram();
    virtual ~ShaderProgram();

    void Load(const Path &vshaderPath, const Path &fshaderPath) override;
    void SetVertexShader(G_Shader *vertexShader) override;
    void SetFragmentShader(G_Shader *fragmentShader) override;

    void SetType(Type type);
    Type GetType() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Type m_type = Type::GBuffer;

    void OnPreLink() override;
    void RetrieveType(const Path &vshaderPath, const Path &fshaderPath);
};

#endif // SHADERPROGRAM_H
