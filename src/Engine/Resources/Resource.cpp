#include "Bang/Resource.h"

#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

Resource::Resource() {}
Resource::~Resource() {}

Path Resource::GetResourceFilepath() const
{
    return ImportFilesManager::GetFilepath( GetGUID() );
}


