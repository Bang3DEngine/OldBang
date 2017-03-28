#ifndef QTPROJECTMANAGER_H
#define QTPROJECTMANAGER_H

#include "Bang/String.h"

/**
 * @brief The QtProjectManager class handles the creation of the .pro file that
 * will let the user edit the code with auto-completion using QtCreator.
 */
class QtProjectManager
{
public:
    static String GetQtProjectFilepath();

    static void CreateQtProjectFile();
    static void OpenBehaviourInQtCreator(const String &behaviourFilepath);

private:
    QtProjectManager();

    static String GetQtProjectFilepathDir();
    static String GetQtProjectFilepathPrefix();

    static bool IsQtCreatorOpenedCurrentProject();
};

#endif // QTPROJECTMANAGER_H
