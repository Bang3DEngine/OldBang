#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/G_Shader.h"

class ShaderManager
{
public:
    ShaderManager();
    static ShaderManager *GetInstance();

    static G_Shader* Load(G_Shader::Type type, const Path& filepath);

    static void RegisterUsageOfShader(G_ShaderProgram *shaderProgram,
                                      G_Shader *shaderBeingUsed);
    static void UnRegisterUsageOfShader(G_ShaderProgram *shaderProgram,
                                        G_Shader *shaderBeingUsed);

private:
    Map<Path, G_Shader*> m_filepathToShaders;
    Map<G_Shader*, Set<G_ShaderProgram*> > m_shaderUsages;
    void Refresh();
};

#endif // SHADERMANAGER_H
