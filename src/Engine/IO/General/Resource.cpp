#include "Bang/Resource.h"

#include "Bang/ImportFilesManager.h"

Resource::Resource() {}
Resource::~Resource() {}

Path Resource::GetResourceFilepath() const
{
    return ImportFilesManager::GetFilepath( GetGUID() );
}
