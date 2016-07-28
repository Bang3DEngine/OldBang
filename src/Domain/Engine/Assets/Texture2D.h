#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Bang.h"

#include <string>
#include <vector>
#include <stack>

#include "glm/glm.hpp"

#include "FileReader.h"
#include "Texture.h"
#include "Logger.h"
#include "Asset.h"

class Texture2D : public Texture, public Asset
{

private:
    //NON COPYABLE:
    Texture2D(const Texture2D &t) : Texture2D() {}


public:
    const static std::string GetFileExtensionStatic() { return "btex2d"; }
    const virtual std::string GetFileExtension()
    {
        return Texture2D::GetFileExtensionStatic();
    }


public:
    Texture2D();
    Texture2D(const std::string &m_filepath);
    virtual ~Texture2D();

    void LoadFromFile(const std::string &m_filepath);
    void CreateEmpty(int m_width, int m_height) override;
    void Resize(int m_width, int m_height) override;
    void Fill(unsigned char *newData, int m_width, int m_height, int m_numComponents);

    std::string GetImageRelativeFilepath() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // TEXTURE2D_H
