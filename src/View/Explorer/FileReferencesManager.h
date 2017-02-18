#ifndef FILEREFERENCESMANAGER_H
#define FILEREFERENCESMANAGER_H

#include <set>
#include <queue>

#include <QObject>

#include "String.h"

/**
 * We must delay the file reference refactoring to let the explorer actually
 * move/rename the files.
 */
class IFileable;
class FileReferencesManager : public QObject
{
    Q_OBJECT

public:
    void RegisterIFileable(IFileable *fileable);
    void UnRegisterIFileable(IFileable *fileable);

    void OnFileOrDirNameAboutToBeChanged(const String &absFilepathBefore,
                                         const String &absFilepathNow);

private:
    // Queue with the changes we are asked for to refactor references.
    // Its a queue of pairs (oldAbsPath, newAbsPath)
    std::queue< std::pair<String, String> > m_queuedNameChanges;
    std::set<IFileable*> m_inMemoryFileables;

    FileReferencesManager();
    virtual ~FileReferencesManager();

    static FileReferencesManager *GetInstance();

    void RefactorFiles(const String &relPathBefore, const String &relPathNow);
    void RefactorIFileables(const String &relPathBefore,
                            const String &relPathNow);

private slots:
    void TreatNextQueuedFileOrDirNameChange();

    friend class Explorer;
    friend class IFileable;
};

#endif // FILEREFERENCESMANAGER_H
