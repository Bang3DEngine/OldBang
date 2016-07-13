#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include "Logger.h"
#include "StringUtils.h"
#include "SystemUtils.h"

class GameBuilder
{
private:
    GameBuilder() {}

public:
    static bool BuildGame(const std::string &absoluteDir);
};

#endif // GAMEBUILDER_H
