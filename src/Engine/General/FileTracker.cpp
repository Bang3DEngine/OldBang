#include "Bang/FileTracker.h"

#include "Bang/Time.h"
#include "Bang/List.h"
#include "Bang/Debug.h"

USING_NAMESPACE_BANG

FileTracker::FileTracker()
{
}

FileTracker::~FileTracker()
{
}

void FileTracker::TrackPath(const Path &path)
{
    if (path.IsDir())
    {
        List<Path> dirSubPaths = path.GetSubPaths(Path::FindFlag::RecursiveHidden);
        for (const Path &subPath : dirSubPaths) { TrackPath(subPath); }
    }

    if (path.Exists())
    {
        if (!m_pathsToTrackToModificationTime.ContainsKey(path))
        {
            m_pathsToTrackToModificationTime.Add(path,
                                                 path.GetModificationTimeSeconds());
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathAdded, path);
        }
    }
}

void FileTracker::UnTrackPath(const Path &path)
{
    if (path.IsDir())
    {
        List<Path> subpaths = path.GetSubPaths(Path::FindFlag::RecursiveHidden);
        for (const Path &subpath : subpaths)
        {
            UnTrackPath(subpath);
        }
    }
    m_pathsToTrackToModificationTime.Remove(path);
}

void FileTracker::Clear()
{
    m_pathsToTrackToModificationTime.Clear();
}

void FileTracker::Update()
{
    float passedSecondsSinceLastCheck = (Time::GetNow_Seconds() - m_lastCheckTime);
    if (passedSecondsSinceLastCheck > GetCheckFrequencySeconds())
    {
        CheckForChanges();
    }
}

void FileTracker::CheckForChanges()
{
    Map<Path, uint64_t> previousPathsToTrack = m_pathsToTrackToModificationTime;

    // Check for removed paths
    for (const auto &previousPathToModTime : previousPathsToTrack)
    {
        const Path &previousPath = previousPathToModTime.first;
        if (!previousPath.Exists())
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathRemoved,
                                    previousPath);
            UnTrackPath(previousPath);
        }
    }

    // Create for new paths and add them to track
    for (const auto &pathToModTime : previousPathsToTrack)
    {
        const Path &path = pathToModTime.first;
        TrackPath(path);
    }


    // Check for modified paths
    for (const auto &newPathToModTime : m_pathsToTrackToModificationTime)
    {
        const Path &newPath = newPathToModTime.first;
        double epsilon = GetCheckFrequencySeconds() / 2.0;
        ASSERT(epsilon < GetCheckFrequencySeconds());
        if (newPath.GetModificationTimeSeconds() >= m_lastCheckTime - epsilon)
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathModified, newPath);
        }
    }

    m_lastCheckTime = SCAST<uint64_t>(Time::GetNow_Seconds());
}

void FileTracker::SetCheckFrequencySeconds(float checkFrequencySeconds)
{
    m_checkFrequencySeconds = checkFrequencySeconds;
}

float FileTracker::GetCheckFrequencySeconds() const
{
    return m_checkFrequencySeconds;
}
