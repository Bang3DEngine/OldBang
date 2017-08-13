#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Bang/Material.h"

class MaterialFactory
{
public:
    static const Material* GetDefault();
    static const Material* GetMissing();

    MaterialFactory() = delete;
};

#endif // MATERIALFACTORY_H
