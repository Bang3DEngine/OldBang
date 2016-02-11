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
public:
    enum FilterMode
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR
    };

    enum WrapMode
    {
        Repeat = GL_REPEAT,
        Clamp = GL_CLAMP
    };

private:
    int width;
    int height;
    int numComponents;

    FilterMode filterMode;
    WrapMode wrapMode;
    int textureSlot;

    unsigned char *rgbs;

public:
    Texture();
    Texture(const std::string &filepath);
    virtual ~Texture();

    void LoadFromFile(const std::string &filepath);
    void CreateEmpty(int width, int height);

    void SetFilterMode(FilterMode filterMode);
    void SetWrapMode(WrapMode wrapMode);
    void SetTextureSlot(int textureSlot);

    FilterMode GetFilterMode() const;
    WrapMode GetWrapMode() const;
    int GetTextureSlot() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // TEXTURE_H
