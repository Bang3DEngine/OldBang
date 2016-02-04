#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Logger.h"
#include "Idable.h"
#include "IToString.h"

class Shader : public IToString, public Idable
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

    Shader(Type t);
    Shader(Type t, const std::string &filepath);

    bool LoadFromFile(const std::string &filepath);

    Type GetType() { return type; }
    const std::string ToString() const;

};

#endif // SHADER_H
