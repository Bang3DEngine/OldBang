#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Part.h"

class Behaviour : public Part
{
public:
    Behaviour();

    virtual const std::string ToString() const override;
};

#endif
