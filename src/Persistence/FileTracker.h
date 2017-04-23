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
#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/String.h"

class FileTracker : public QObject
{
    Q_OBJECT

public:
    FileTracker();
    static FileTracker *GetInstance();
    List< std::pair<String,String> > GetMovedPathsList() const;
    static void TrackFilesWithExtension(const String& extension);
    static bool HasFileChanged(const String& absFilepath, EpochTime since);

signals:
    void ChangedFilesRefreshed();

private slots:
    void Refresh();
    void ReloadChangedFiles();

private:
    static constexpr int c_refreshTime = 3000;

    List<String> m_additionalExtensions;

    mutable List<String> m_lastPathsList;
    Map<String, EpochTime> m_lastModifTimes;
    Map<String, EpochTime> m_lastSeenTimes;
    Map<String, List<String> > m_fileDependencies;

    // The refreshing updates the queue of changed files.
    // Works in a separate thread
    QTimer m_refreshTimer;
    QThread m_refreshThread;
    EpochTime m_timeBeforeRefreshing;

    List<String> GetPathsToTrack() const;
    bool MustTrackFile(const File& file) const;
    void RefreshFileModificationDate(const String& absFilepath);
    void RefreshFileDependencies(const String& absFilepath);
    bool HasFileChanged(const String& absFilepath) const;
    bool HasFileChanged(const String& absFilepath,
                        Set<String>& alreadyCheckedFiles) const;

    static bool HasFileChanged(EpochTime lastSeenTime,
                               EpochTime modificationTime);
};

#endif // FILETRACKER_H
