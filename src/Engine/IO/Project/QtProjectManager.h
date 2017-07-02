#ifndef QTPROJECTMANAGER_H
#define QTPROJECTMANAGER_H

#include "Bang/Path.h"
#include "Bang/String.h"

/**
 * @brief The QtProjectManager class handles the creation of the .pro file that
 * will let the user edit the code with auto-completion using QtCreator.
 */
class QtProjectManager
{
public:
    static Path GetQtProjectFilepath();

    static void CreateQtProjectFile();
    static void OpenBehaviourInQtCreator(const Path &behaviourFilepath);

private:
    QtProjectManager();

    static Path GetQtProjectDir();
    static bool IsQtCreatorOpenedCurrentProject();
};

#endif // QTPROJECTMANAGER_H
