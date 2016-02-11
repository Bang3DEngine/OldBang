#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <vector>
#include <stack>

#include "glm/glm.hpp"


#include "FileLoader.h"
#include "Texture.h"
#include "Logger.h"

class Texture2D : public Texture
{

public:
    Texture2D();
    Texture2D(const std::string &filepath);
    virtual ~Texture2D();

    void LoadFromFile(const std::string &filepath);
    void CreateEmpty(int width, int height) override;
};

#endif // TEXTURE2D_H
