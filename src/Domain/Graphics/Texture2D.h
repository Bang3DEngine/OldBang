#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <vector>
#include <stack>

#include "glm/glm.hpp"


#include "FileReader.h"
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
    void Resize(int width, int height) override;
    void Fill(unsigned char *newData, int width, int height, int numComponents);
};

#endif // TEXTURE2D_H
