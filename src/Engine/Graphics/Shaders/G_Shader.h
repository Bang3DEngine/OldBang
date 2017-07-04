#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include "Bang/Array.h"
#include "Bang/Asset.h"
#include "Bang/GLObject.h"
#include "Bang/IToString.h"

class G_Shader : public GLObject,
                 public IToString
{
public:
    enum class Type
    {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

    G_Shader(G_Shader::Type t);
    G_Shader(G_Shader::Type t, const Path &shaderPath);

    bool LoadFromFile(const Path &filepath);

    GL::BindTarget GetGLBindTarget() const override;

    const String& GetSourceCode() const;
    const Path& GetFilepath() const;
    Type GetType() const;
    String ToString() const;

private:
    String m_sourceCode = "";
    Path m_filepath;
    Type m_type;

};

#endif // SHADER_H
