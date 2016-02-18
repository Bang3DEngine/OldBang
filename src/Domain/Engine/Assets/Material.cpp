#include "Material.h"

Material::Material() : Asset(),
                       shaderProgram(nullptr)
{
}

Material::~Material()
{

}

void Material::Bind() const
{
    if(shaderProgram != nullptr)
    {
        shaderProgram->Bind();

        for(unsigned int i = 0; i < textures.size(); ++i)
        {
            const Texture *t = textures[i];
            if(t != nullptr)
            {
                std::string texName = ShaderContract::Uniform_Texture_Prefix + std::to_string(i);
                shaderProgram->SetUniformTexture(texName, t, false); //Set the uniform with the texture slot
                t->Bind(); //Leave it bound
            }
        }
    }
}

void Material::UnBind() const
{
    if(shaderProgram != nullptr)
    {
        shaderProgram->UnBind();
        for(unsigned int i = 0; i < textures.size(); ++i)
        {
            if(textures[i] != nullptr)
                textures[i]->UnBind();
        }
    }
}

void Material::Write(std::ofstream &f) const
{

}

void Material::Read(std::ifstream &f)
{
    AssetsReader::RegisterNextPointerId(f, this);
    filepath = FileReader::ReadString(f);

    //Read the file itself, where the material is defined (*.mat)
    std::ifstream fm (filepath);
    if ( !fm.is_open() )
    {
        Logger_Error("Could not open the file '" << filepath << "' to load the material.");
    }
    else
    {
        FileReader::ReadNextLine(fm); // Skip <Material> line

        //Read the ShaderProgram
        SetShaderProgram(new ShaderProgram(ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv,
                                           ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv));
        //Read the texture
        Texture2D *tex = new Texture2D( FileReader::ReadString(fm) );
        tex->SetTextureSlot(0);
        SetTexture(tex);

        FileReader::ReadNextLine(fm); // Skip </Material> line
    }

    FileReader::ReadNextLine(f); //Consume close tag
}

void Material::SetShaderProgram(const ShaderProgram *program)
{
    shaderProgram = program;
}

void Material::SetTexture(const Texture *texture, unsigned int index)
{
    while(textures.size() <= index) textures.push_back(nullptr);
    textures[index] = texture;
}

const ShaderProgram *Material::GetShaderProgram() const
{
    return shaderProgram;
}

const Texture *Material::GetTexture(unsigned int index) const
{
    if(index >= textures.size()) return nullptr;
    return textures[index];
}
