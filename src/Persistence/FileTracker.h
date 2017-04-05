#ifndef FILETRACKER_H
#define FILETRACKER_H

#include <QTimer>
#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/List.h"
#include "Bang/String.h"

class FileTracker : public QObject
{
    Q_OBJECT

    typedef qint64 EpochTime;

public:
    FileTracker();

private slots:
    void Refresh();

private:
    Map<String, EpochTime> m_fileChangeTimes;
    Map<String, EpochTime> m_fileSeenTimes;
    Map<String, List<String> > m_fileDependencies;
    QTimer m_refreshTimer;

    void RefreshFileModificationDate(const String& absFilepath);
    void RefreshFileDependencies(const String& absFilepath);
    void ReloadChangedFiles(const List<String>& allFiles) const;
    bool HasChanged(const String& absFilepath) const;
    bool HasChanged(const String& absFilepath,
                    Set<String>& alreadyCheckedFiles) const;
};

#endif // FILETRACKER_H
