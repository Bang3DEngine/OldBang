#ifndef COLLECTION_H
#define COLLECTION_H

#include "Bang/ICollection.h"
#include "Bang/AnyIterator.h"

// Intermidiate class implementing some helping functions for the ICollection
// interface
template<class T>
class Collection : public ICollection
{
protected:
    Collection()
    {
    }

    virtual ~Collection()
    {
    }
};

#endif // COLLECTION_H
