#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include "Bang/Path.h"
#include "Bang/BinType.h"
#include "Bang/Compiler.h"

class QLibrary;
class Behaviour;
/**
 * @brief Manages the compiling and loading of the Behaviour's Libraries.
 */
class BehaviourManager
{
public:
    BehaviourManager();

    static Compiler::Result CompileBehaviourObject(
                                       const Path& behaviourFilepath,
                                       const Path& outputObjectFilepath,
                                       BinType binaryType);

    static Compiler::Result MergeBehaviourObjects(
                                    const Path& outputLibFilepath,
                                    const List<Path> &behaviourObjectFilepaths,
                                    BinType binaryType);

    static QLibrary *GetBehavioursLibrary();
    static void LoadBehavioursLibrary(const Path &behavioursLibrary);

    static void RemoveOldBehaviourLibraries(const Path& librariesDir);

private:
    QLibrary *m_behavioursLibrary = nullptr;

    static Compiler::Job CreateBaseJob(BinType binaryType);

    static BehaviourManager* GetInstance();
};

#endif // BEHAVIOURMANAGER_H
