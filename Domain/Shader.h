#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Logger.h"
#include "GLIdable.h"
#include "IToString.h"

class Shader : public IToString, public GLIdable
{
public:

    enum Type {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

private:
    std::string sourceCode;
    Type type;

public:

    Shader(Shader::Type t);

    bool LoadFromFile(const std::string &filepath);

    Type GetType() const { return type; }
    const std::string GetSourceCode() const { return sourceCode; }
    const std::string ToString() const;
};

#endif // SHADER_H
