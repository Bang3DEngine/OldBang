#ifndef OCTREE_H
#define OCTREE_H

#include <array>

#include "Bang/Bang.h"
#include "Bang/AABox.h"
#include "Bang/Array.h"

NAMESPACE_BANG_BEGIN

template <class T, class ClassifyFunctor>
class Octree
{
public:
    Octree();
    ~Octree();

    void SetAABox(const AABox& aabox);
    uint Fill(const Array<T> &elements, uint maxDepth);

    const AABox GetAABox() const;
    const Array<T>& GetElements() const;
    Array<T> GetElementsRecursive() const;
    const std::array<Octree*, 8>& GetChildren() const;
    Array<const Octree*> GetChildrenAtLevel(
                            uint level,
                            bool includeEarlyPrunedInPreviousLevels) const;

private:
    Array<T> m_leafElements; // Only filled if we are leaf node
    AABox m_aaBox;           // AABox of this octree

    std::array<Octree*, 8> m_children; // Octree children
};

NAMESPACE_BANG_END

#include "Bang/Octree.tcc"

#endif // OCTREE_H

