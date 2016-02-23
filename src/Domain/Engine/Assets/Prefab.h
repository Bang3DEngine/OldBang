#ifndef BANGFAB_H
#define BANGFAB_H

#include "Bang.h"

#include <string>

#include "Entity.h"
#include "Asset.h"

//Represents an Entity that can be saved as an asset, and
//can be instantiated.
//In fact its asset file will be of the form
//<Entity>
//  ...
//</Entity>
class Prefab : public Asset
{
private:
    //This variable contains the description inside the
    //corresponding *.asset file (without the opening/closing tags)
    std::string assetDescription;

public:
    Prefab();
    Prefab(const std::string &assetDescription);

    Entity* Instantiate() const;

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // BANGFAB_H
