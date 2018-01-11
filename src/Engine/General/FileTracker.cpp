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
    if (path.IsDir())
    {
        List<Path> dirSubPaths = path.GetSubPaths(Path::FindFlag::RecursiveHidden);
        for (const Path &subPath : dirSubPaths) { AddPath(subPath); }
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

void FileTracker::RemovePath(const Path &path)
{
    if (path.IsDir())
    {
        List<Path> subpaths = path.GetSubPaths(Path::FindFlag::RecursiveHidden);
        for (const Path &subpath : subpaths)
        {
            RemovePath(subpath);
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

    // Create the new path tracking list
    for (const auto &pathToModTime : previousPathsToTrack)
    {
        const Path &path = pathToModTime.first;
        AddPath(path); // Add m_pathsToTrackToModificationTime
    }

    // Update modification times
    for (auto &pathToModTime : m_pathsToTrackToModificationTime)
    {
        pathToModTime.second = pathToModTime.first.GetModificationTimeSeconds();
    }

    // Check for new and modified paths
    for (const auto &newPathToModTime : m_pathsToTrackToModificationTime)
    {
        const Path &newPath = newPathToModTime.first;
        if (!previousPathsToTrack.ContainsKey(newPath))
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathAdded, newPath);
        }

        double epsilon = GetCheckFrequencySeconds() / 2.0;
        if (newPath.GetModificationTimeSeconds() >= m_lastCheckTime - epsilon)
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathModified, newPath);
        }

    }

    // Check for removed paths
    for (const auto &previousPathToModTime : previousPathsToTrack)
    {
        const Path &previousPath = previousPathToModTime.first;
        if (!m_pathsToTrackToModificationTime.ContainsKey(previousPath))
        {
            EventEmitter<IFileTrackerListener>::
               PropagateToListeners(&IFileTrackerListener::OnPathRemoved, previousPath);
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
