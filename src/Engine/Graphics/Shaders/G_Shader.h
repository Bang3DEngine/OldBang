#ifndef G_SHADER_H
#define G_SHADER_H

#include <GL/glew.h>

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Resource.h"
#include "Bang/GLObject.h"

class G_Shader : public GLObject,
                 public Resource
{
    RESOURCE(G_Shader)

public:
    enum class Type
    {
        Vertex   = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

    G_Shader();
    G_Shader(G_Shader::Type t);

    bool LoadFromFile(const Path &filepath);

    GL::BindTarget GetGLBindTarget() const override;

    const String& GetSourceCode() const;
    const Path& GetFilepath() const;
    Type GetType() const;

    virtual void PreprocessCode(String *shaderSrc) const;

protected:
    String m_sourceCode = "";
    Path m_filepath;
    Type m_type;

};

#endif // G_SHADER_H
