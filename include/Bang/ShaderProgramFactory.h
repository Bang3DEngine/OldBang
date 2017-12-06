#ifndef SHADERPROGRAMFACTORY_H
#define SHADERPROGRAMFACTORY_H

#include "Bang/ShaderProgram.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class ShaderProgramFactory
{
public:
    ShaderProgramFactory();

    static ShaderProgram* GetDefault();
    static ShaderProgram* Get(const Path &vShaderPath, const Path &fShaderPath);

private:
    Map< std::pair<Path, Path>, RH<ShaderProgram> > m_cache;

    static ShaderProgramFactory* GetActive();
};

NAMESPACE_BANG_END

#endif // SHADERPROGRAMFACTORY_H

