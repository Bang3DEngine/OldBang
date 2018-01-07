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

void FileTracker::AddPath(const Path &path)
{
    m_pathsToTrackToModificationTime.Add(path, path.GetModificationTimeNanos());
}

void FileTracker::Clear()
{
    m_pathsToTrackToModificationTime.Clear();
}

void FileTracker::Update()
{
    float passedSecondsSinceLastCheck =
            SCAST<double>(Time::GetNow_Nanos() - m_lastCheckTime) / 1e9;
    if (passedSecondsSinceLastCheck > GetCheckFrequencySeconds())
    {
        CheckForChanges();
    }
}

void FileTracker::CheckForChanges()
{
    m_lastCheckTime = Time::GetNow_Nanos();

    Map<Path, uint64_t>& previousPathsToTrack = m_pathsToTrackToModificationTime;

    // Create the new path tracking list
    Map<Path, uint64_t> newPathsToTrack = m_pathsToTrackToModificationTime;
    for (const auto &pathToModTime : m_pathsToTrackToModificationTime)
    {
        const Path &path = pathToModTime.first;
        if (path.IsDir())
        {
            List<Path> dirSubPaths = path.FindSubPaths(Path::FindFlag::RecursiveHidden);
            for (const Path &subPath : dirSubPaths)
            {
                newPathsToTrack.Add(subPath);
            }
        }
        else if (!path.Exists())
        {
            newPathsToTrack.Remove(path);
        }
    }

    // Check for new and modified paths
    for (const auto &newPathToModTime : newPathsToTrack)
    {
        const Path &newPath = newPathToModTime.first;
        if (!previousPathsToTrack.ContainsKey(newPath))
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathAdded, newPath);
        }

        uint64_t epsilon = GetCheckFrequencySeconds() * 1e9 / 2;
        if (newPath.GetModificationTimeNanos() >= m_lastCheckTime - epsilon)
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathModified, newPath);
        }

    }

    for (const auto &previousPathToModTime : previousPathsToTrack)
    {
        const Path &previousPath = previousPathToModTime.first;
        if (!newPathsToTrack.ContainsKey(previousPath))
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathRemoved, previousPath);
        }
    }

    m_pathsToTrackToModificationTime = newPathsToTrack;
}

void FileTracker::SetCheckFrequencySeconds(float checkFrequencySeconds)
{
    m_checkFrequencySeconds = checkFrequencySeconds;
}

float FileTracker::GetCheckFrequencySeconds() const
{
    return m_checkFrequencySeconds;
}
