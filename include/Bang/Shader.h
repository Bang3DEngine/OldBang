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
    const Path& GetResourceFilepath() const;
    GL::ShaderType GetType() const;

    // Resource
    virtual void Import(const Path &shaderFilepath) override;

protected:
    String m_sourceCode = "";
    Path m_filepath;
    GL::ShaderType m_type;
};

NAMESPACE_BANG_END

#endif // SHADER_H
