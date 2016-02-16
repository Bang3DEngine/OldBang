#ifndef STAGELOADER_H
#define STAGELOADER_H

#include <istream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <stack>

#include "glm/glm.hpp"

#include "Behaviour.h"
#include "Transform.h"
#include "Logger.h"
#include "Stage.h"

class StageLoader
{
private:
    StageLoader() {}
    static void TrimStringLeft(std::string *str);
    static std::string GetLine(std::ifstream &f);

    static float ReadFloat(std::ifstream &f);
    static glm::vec2 ReadVec2(std::ifstream &f);
    static glm::vec3 ReadVec3(std::ifstream &f);
    static glm::quat ReadQuat(std::ifstream &f);
    static std::string ReadString(std::ifstream &f);

    static std::string ReadTag(std::string &line);

    //They all start from the first line inside the tag
    // (you must consume the initial tag),
    // and they consume the closing tag.
    static Transform* ReadTransform(std::ifstream &f);
    static Entity* ReadEntity(std::ifstream &f);

public:
    static void LoadStage(const std::string &filepath, Stage* stage);
    static void SaveStage(const std::string &filepath, const Stage *stage);
};

#endif // STAGELOADER_H
