#ifndef STAGEREADER_H
#define STAGEREADER_H

#include <istream>
#include <sstream>
#include <functional>
#include <fstream>
#include <string>
#include <stack>
#include <map>

#include "ReaderWithPointerIds.h"
#include "glm/glm.hpp"
#include "Rect.h"

#include "BP_StageReader_cpp_UserBehaviours_elseifs.bp"

class Stage;
class Entity;
class StageReader : public ReaderWithPointerIds
{
private:

    StageReader() {}

public:

    //They all start from the first line inside the tag
    // (you must consume the initial tag),
    // and they consume the closing tag.

    //PARTS
    static void ReadParts(std::istream &f, Entity *e);

    //ENTITIES
    static void ReadChildren(std::istream &f, Entity *e);
    // ////////////////////

public:
    static void ReadStage(const std::string &filepath, Stage* stage);
    static void SaveStage(const std::string &filepath, const Stage *stage);

};

#endif // STAGEREADER_H
