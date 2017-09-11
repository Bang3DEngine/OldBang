#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Resource.h"
#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

class Shader : public GLObject,
               public Resource
{
    RESOURCE(Shader)

public:
    Shader();
    Shader(GL::ShaderType t);

    GL::BindTarget GetGLBindTarget() const override;

    const String& GetSourceCode() const;
    GL::ShaderType GetType() const;

    // Resource
    virtual void Import(const Path &shaderFilepath) override;

private:
    GL::ShaderType m_type;
    String m_sourceCode = "";

    void RetrieveType(const Path &shaderPath);
};

NAMESPACE_BANG_END

#endif // SHADER_H
