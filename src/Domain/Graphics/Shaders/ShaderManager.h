#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QTimer>
#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/String.h"
#include "Bang/Shader.h"

class ShaderManager : public QObject
{
    Q_OBJECT

public:
    ShaderManager();
    static ShaderManager *GetInstance();

    static Shader* Load(Shader::Type type, const Path& filepath);

    static void RegisterUsageOfShader(ShaderProgram *shaderProgram,
                                      Shader *shaderBeingUsed);
    static void UnRegisterUsageOfShader(ShaderProgram *shaderProgram,
                                        Shader *shaderBeingUsed);

private:
    QTimer m_refreshTimer;
    EpochTime m_lastRefreshTime;

    Map<Path, Shader*> m_filepathToShaders;
    Map<Shader*, Set<ShaderProgram*> > m_shaderUsages;

private slots:
    void Refresh();
};

#endif // SHADERMANAGER_H
