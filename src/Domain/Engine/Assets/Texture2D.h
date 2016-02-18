#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <vector>
#include <stack>

#include "glm/glm.hpp"


#include "AssetsReader.h"
#include "FileReader.h"
#include "IFileable.h"
#include "Texture.h"
#include "Logger.h"
#include "Asset.h"

class Texture2D : public Texture, public IFileable, public Asset
{

public:
    Texture2D();
    Texture2D(const std::string &filepath);
    virtual ~Texture2D();

    void LoadFromFile(const std::string &filepath);
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(unsigned char *newData, int width, int height, int numComponents);

    // IFileable interface
public:
    void Write(std::ofstream &f) const;
    void Read(std::ifstream &f);
};

#endif // TEXTURE2D_H
