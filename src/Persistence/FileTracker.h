#ifndef FILETRACKER_H
#define FILETRACKER_H

#include <queue>

#include <QTimer>
#include <QObject>
#include <QThread>
#include <QMutexLocker>
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

signals:
    void ChangedFilesRefreshed();

private slots:
    void Refresh();
    void ReloadChangedFiles();

private:
    static constexpr int c_refreshTime = 3000;

    Map<String, EpochTime> m_fileChangeTimes;
    Map<String, EpochTime> m_fileSeenTimes;
    Map<String, List<String> > m_fileDependencies;

    // The refreshing updates the queue of changed files.
    // Works in a separate thread
    QTimer m_refreshTimer;
    QThread m_refreshThread;
    EpochTime m_timeBeforeRefreshing;

    void RefreshFileModificationDate(const String& absFilepath);
    void RefreshFileDependencies(const String& absFilepath);
    bool HasChanged(const String& absFilepath) const;
    bool HasChanged(const String& absFilepath,
                    Set<String>& alreadyCheckedFiles) const;
};

#endif // FILETRACKER_H
