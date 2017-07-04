#ifndef SHADER_H
#define SHADER_H

#include "Bang/Path.h"
#include "Bang/G_Shader.h"
#include "Bang/IToString.h"

class Shader : public G_Shader,
               public IToString
{
public:
    Shader(G_Shader::Type t);

    void PreprocessCode(String *shaderSrc) const override;
    String ToString() const override;
};

#endif // SHADER_H
