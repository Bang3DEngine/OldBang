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
    std::stack<GLuint> lastActiveTextureSlots;

    int width;
    int height;
    int numComponents;

    unsigned char *rgbs;

public:
    Texture();
    virtual ~Texture();

    void LoadFromFile(const std::string &filepath);

    void Bind() const override;
    void UnBind() const override;
};

#endif // TEXTURE_H
