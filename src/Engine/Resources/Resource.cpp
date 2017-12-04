#include "Bang/Resource.h"

#include "Bang/Resources.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

// IResource
IResource::IResource()
{
    ASSERT(Resources::AssertCreatedFromResources());
}

IResource::~IResource()
{
    ASSERT(Resources::AssertDestroyedFromResources());
}

// Resource
Path Resource::GetResourceFilepath() const
{
    return ImportFilesManager::GetFilepath( GetGUID() );
}
