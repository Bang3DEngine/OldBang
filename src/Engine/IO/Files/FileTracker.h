#ifndef FILETRACKER_H
#define FILETRACKER_H

#include <queue>

#include <QTimer>
#include <QObject>
#include <QThread>
#include <QMutexLocker>

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/BFile.h"
#include "Bang/String.h"

class FileTracker : public QObject
{
    Q_OBJECT

public:
    FileTracker();
    static FileTracker *GetInstance();
    List< std::pair<Path,Path> > GetMovedPathsList() const;
    static void TrackFilesWithExtension(const String& extension);
    static bool HasFileChanged(const Path& filepath, EpochTime since);

signals:
    void ChangedFilesRefreshed();

private slots:
    void Refresh();
    void ReloadChangedFiles();

private:
    static constexpr int c_refreshTime = 3000;

    List<String> m_additionalExtensions;

    mutable List<Path> m_lastPathsList;
    Map<Path, EpochTime> m_lastModifTimes;
    Map<Path, EpochTime> m_lastSeenTimes;
    Map<Path, List<Path> > m_fileDependencies;

    // The refreshing updates the queue of changed files.
    // Works in a separate thread
    QTimer m_refreshTimer;
    QThread m_refreshThread;
    EpochTime m_timeBeforeRefreshing;

    List<Path> GetPathsToTrack() const;
    bool MustTrackFile(const BFile& file) const;
    void RefreshFileModificationDate(const Path& filepath);
    void RefreshFileDependencies(const Path& filepath);
    bool HasFileChanged(const Path& filepath) const;
    bool HasFileChanged(const Path& filepath,
                        Set<Path>& alreadyCheckedFiles) const;

    static bool HasFileChanged(EpochTime lastSeenTime,
                               EpochTime modificationTime);
};

#endif // FILETRACKER_H
