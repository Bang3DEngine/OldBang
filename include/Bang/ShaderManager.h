#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/Shader.h"

NAMESPACE_BANG_BEGIN

class ShaderManager
{
public:
    ShaderManager();
    static ShaderManager *GetInstance();

    static Shader* Load(GL::ShaderType type, const Path& filepath);

    static void RegisterUsageOfShader(ShaderProgram *shaderProgram,
                                      Shader *shaderBeingUsed);
    static void UnRegisterUsageOfShader(ShaderProgram *shaderProgram,
                                        Shader *shaderBeingUsed);

private:
    Map<Path, Shader*> m_filepathToShaders;
    Map<Shader*, Set<ShaderProgram*> > m_shaderUsages;
    void Refresh();
};

NAMESPACE_BANG_END

#endif // SHADERMANAGER_H
