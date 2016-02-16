#ifndef STAGELOADER_H
#define STAGELOADER_H

#include <string>

#include "Stage.h"

class StageLoader
{
private:
    StageLoader() {}

public:
    static void LoadStage(const std::string &filepath, Stage* stage);
    static void SaveStage(const std::string &filepath, const Stage *stage);
};

#endif // STAGELOADER_H
