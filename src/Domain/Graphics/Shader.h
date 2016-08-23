#ifndef SHADER_H
#define SHADER_H

#include "Bang.h"

#include <GL/glew.h>

#include <vector>

#include <sstream>
#include <fstream>
#include <iostream>

#include "Debug.h"
#include "IGLIdable.h"
#include "IToString.h"
#include "ShaderPreprocessor.h"

class Shader : public IToString, public IGLIdable
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
    const String ToString() const;
};

#endif // SHADER_H
