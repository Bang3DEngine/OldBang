#ifndef OCTREE_TCC
#define OCTREE_TCC

#include <array>
#include <queue>

#include "Bang/Debug.h"
#include "Bang/Octree.h"

USING_NAMESPACE_BANG

template<class T, class ClassifyFunctor>
Octree<T, ClassifyFunctor>::Octree()
{
    for (int i = 0; i < 8; ++i) { m_children[i] = nullptr; }
}

template<class T, class ClassifyFunctor>
Octree<T, ClassifyFunctor>::~Octree()
{
    for (int i = 0; i < 8; ++i)
    {
        Octree *childOct = m_children[i];
        if (childOct) { delete childOct; }
    }
}

template<class T, class ClassifyFunctor>
void Octree<T, ClassifyFunctor>::SetAABox(const AABox &aabox)
{
    m_aaBox = aabox;
    m_leafElements.Clear();
}

template<class T, class ClassifyFunctor>
uint Octree<T, ClassifyFunctor>::Fill(const Array<T> &elements, uint maxDepth)
{
    // Returns the number of contained elements after filling
    if (maxDepth < 0) { return -1; }

    // Get elements inside me
    Array<T> containedElements;
    ClassifyFunctor classifyFunctor;
    for (const T& element : elements)
    {
        if (classifyFunctor(GetAABox(), element))
        {
            // Debug_Peek(element);
            containedElements.PushBack(element);
        }
    }

    if (maxDepth > 0)
    {
        // Keep subdividing.
        if (containedElements.Size() > 1)
        {
            // Create and fill children if we contain more than one element.
            const Vector3 minPoint = GetAABox().GetMin();
            const Vector3 size     = GetAABox().GetSize();
            const Vector3 &mp      = minPoint;
            const Vector3 hs       = size / 2.0f;

            std::array<Vector3, 8> sizeDirs = {{
              Vector3(0,0,0), Vector3(0,0,1), Vector3(0,1,0), Vector3(0,1,1),
              Vector3(1,0,0), Vector3(1,0,1), Vector3(1,1,0), Vector3(1,1,1)}};
            for (int i = 0; i < 8; ++i)
            {
                const Vector3& sizeDir = sizeDirs[i];

                Octree *childOctree = new Octree<T, ClassifyFunctor>();
                childOctree->SetAABox(
                            AABox::FromPointAndSize(mp + hs * sizeDir, hs) );
                int childContainedElements =
                        childOctree->Fill(containedElements, maxDepth-1);

                if (childContainedElements > 0) { m_children[i] = childOctree; }
                else { delete childOctree; }
            }
        }
        else if (containedElements.Size() == 1)
        {
            // Otherwise, we only have one element. Add element and dont
            // keep subdividing.
            m_leafElements.PushBack( containedElements.Front() );
        }
    }
    else
    {
        // We arrived to the octree max depth. Just store all the remaining
        // elements in this node
        m_leafElements.PushBack(containedElements);
    }

    return containedElements.Size();
}

template<class T, class ClassifyFunctor>
const AABox Octree<T, ClassifyFunctor>::GetAABox() const
{
    return m_aaBox;
}

template<class T, class ClassifyFunctor>
Array<T> Octree<T, ClassifyFunctor>::GetElementsRecursive() const
{
    Array<T> elements = GetElements();
    for (const Octree *child : GetChildren())
    {
        if (child)
        {
            elements.PushBack( child->GetElementsRecursive() );
        }
    }
    return elements;
}

template<class T, class ClassifyFunctor>
const Array<T>& Octree<T, ClassifyFunctor>::GetElements() const
{
    return m_leafElements;
}

template<class T, class ClassifyFunctor>
const std::array<Octree<T, ClassifyFunctor>*, 8>&
Octree<T, ClassifyFunctor>::GetChildren() const
{
    return m_children;
}

template<class T, class ClassifyFunctor>
Array< const Octree<T, ClassifyFunctor>* >
Octree<T, ClassifyFunctor>::GetChildrenAtLevel(
                                uint level,
                                bool includeEarlyPrunedInPreviousLevels) const
{
    using OctLevelPair = std::pair<uint, const Octree*>;

    Array<const Octree*> childrenAtLevelResult;

    std::queue< OctLevelPair > queuedOctLevelPairs;
    queuedOctLevelPairs.push( std::make_pair(0, this) );

    while (!queuedOctLevelPairs.empty())
    {
        const OctLevelPair &currentOctLevelPair = queuedOctLevelPairs.front();
        const Octree *currentOctree = currentOctLevelPair.second;
        uint currentLevel = currentOctLevelPair.first;
        queuedOctLevelPairs.pop();

        if (currentLevel == level)
        {
            childrenAtLevelResult.PushBack(currentOctree);
        }
        else
        {
            if (includeEarlyPrunedInPreviousLevels &&
                currentOctree->GetElements().Size() == 1)
            {
                childrenAtLevelResult.PushBack(currentOctree);
            }

            for (const Octree *currentChild : currentOctree->GetChildren())
            {
                if (currentChild)
                {
                    queuedOctLevelPairs.push( std::make_pair(currentLevel + 1,
                                                             currentChild) );
                }
            }
        }
    }
    return childrenAtLevelResult;
}

#endif // OCTREE_TCC

