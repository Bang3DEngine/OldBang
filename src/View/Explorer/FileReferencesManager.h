#ifndef FILEREFERENCESMANAGER_H
#define FILEREFERENCESMANAGER_H

#include <queue>

#include <QTimer>
#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/Set.h"
#include "Bang/String.h"

/**
 * We must delay the file reference refactoring to let the explorer actually
 * move/rename the files.
 */
class XMLNode;
class SerializableObject;
class FileReferencesManager : public QObject
{
    Q_OBJECT

public:
    void RegisterSerializableObject(SerializableObject *fileable);
    void UnRegisterSerializableObject(SerializableObject *fileable);

public slots:
    void CheckForMovedFiles();

private:

    QTimer m_timer;

    // Queue with the changes we are asked for to refactor references.
    // Its a queue of pairs (oldAbsPath, newAbsPath)
    Set<SerializableObject*> m_inMemorySerialObjects;

    FileReferencesManager();
    virtual ~FileReferencesManager();

    static FileReferencesManager *GetInstance();

    void RefactorFiles(const String &relPathBefore, const String &relPathNow);
    void RefactorSerializableObject(const String &relPathBefore,
                            const String &relPathNow);

    bool RefactorXMLInfo(XMLNode *xmlInfo,
                         const String &relPathBefore,
                         const String &relPathNow,
                         bool refactorXMLChildren);

    void OnFileOrDirNameMoved(const String &absFilepathBefore,
                                         const String &absFilepathNow);

    friend class Explorer;
    friend class SerializableObject;
};

#endif // FILEREFERENCESMANAGER_H
