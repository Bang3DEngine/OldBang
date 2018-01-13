#ifndef FILETRACKER_H
#define FILETRACKER_H

#include "Bang/Map.h"
#include "Bang/Path.h"
#include "Bang/IEventEmitter.h"

NAMESPACE_BANG_BEGIN

class IFileTrackerListener : public IEventListener
{
public:
    virtual void OnPathAdded(const Path &addedPath)       {}
    virtual void OnPathModified(const Path &modifiedPath) {}
    virtual void OnPathRemoved(const Path &removedPath)   {}
};



class FileTracker : public EventEmitter<IFileTrackerListener>
{
public:
	FileTracker();
	virtual ~FileTracker();

    void TrackPath(const Path &path);
    void UnTrackPath(const Path &path);
    void Clear();

    void Update();
    void CheckForChanges();
    void SetCheckFrequencySeconds(float checkFrequencySeconds);

    float GetCheckFrequencySeconds() const;

private:
    Map<Path, uint64_t> m_pathsToTrackToModificationTime;

    uint64_t m_lastCheckTime = 0.0;
    float m_checkFrequencySeconds = 5.0f;
};

NAMESPACE_BANG_END

#endif // FILETRACKER_H

