#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang/Asset.h"
#include "Bang/G_ShaderProgram.h"

class ShaderProgram : public G_ShaderProgram,
                      public Asset
{
    ASSET(ShaderProgram)

public:
    enum Type
    {
        GBuffer,
        PostProcess,
        Other
    };

    ShaderProgram();
    virtual ~ShaderProgram();

    void SetType(Type type);
    Type GetType() const;

    // Resource
    void Import(const Path &shaderProgramFilepath) override;

    // G_ShaderProgram
    void Load(const Path &vshaderPath, const Path &fshaderPath) override;
    void SetVertexShader(G_Shader *vertexShader) override;
    void SetFragmentShader(G_Shader *fragmentShader) override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Type m_type = Type::GBuffer;

    void RetrieveType(const Path &vshaderPath, const Path &fshaderPath);

    // G_ShaderProgram
    void OnPreLink() override;
};

#endif // SHADERPROGRAM_H
