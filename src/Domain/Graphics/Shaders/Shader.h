#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include "Array.h"

#include "NamedEnum.h"
#include "IGLIdable.h"
#include "IToString.h"

class Shader : public IToString,
               public IGLIdable
{
public:

    NamedEnum (Type,
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    );

private:
    String m_sourceCode;
    String m_filepath;
    Type m_type;

public:

    Shader(Shader::Type t);
    Shader(Shader::Type t, const String &shaderPath);

    bool LoadFromFile(const String &filepath);

    const String& GetSourceCode() const;
    const String& GetFilepath() const;
    Type GetType() const;
    String ToString() const;
};

#endif // SHADER_H
