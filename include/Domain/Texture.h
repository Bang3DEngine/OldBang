#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <stack>

#include "glm/glm.hpp"


#include "FileLoader.h"
#include "GLBindable.h"
#include "GLIdable.h"
#include "Logger.h"

class Texture : public GLBindable, public GLIdable
{
private:
    int width;
    int height;
    int textureSlot;
    int numComponents;

    unsigned char *rgbs;

public:
    Texture(int textureSlot);
    virtual ~Texture();

    void LoadFromFile(const std::string &filepath);

    int GetTextureSlot() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // TEXTURE_H
