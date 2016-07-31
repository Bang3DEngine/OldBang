#ifndef MATERIALASSETFILE_H
#define MATERIALASSETFILE_H

#include "Bang.h"

#include "File.h"
#include "XMLParser.h"

#include <string>
#include <vector>

class MaterialAssetFile : public File
{
private:
    std::string m_vshaderFilepath = "";
    std::string m_fshaderFilepath = "";
    int m_textureCount = 0;
    std::vector< std::string > m_textureFilepaths;
    glm::vec4 m_diffuseColor = glm::vec4(0.0f);

public:
    MaterialAssetFile(const QFileSystemModel *model, const QModelIndex *index);

    const std::string& GetVertexShaderFilepath() const;
    const std::string& GetFragmentShaderFilepath() const;
    int GetTextureCount() const;
    const std::vector<std::string>& GetTextureFilepaths() const;
    const glm::vec4& GetDiffuseColor() const;
};

#endif // MATERIALASSETFILE_H
