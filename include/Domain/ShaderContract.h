#ifndef SHADERCONTRACT_H
#define SHADERCONTRACT_H

//TODO: Add check of all variables beginning with ShaderNames_Prefix, to see
//      if all of them have an existing name. Do this when loading shaders.

#include <string>

class ShaderContract
{
private:
    ShaderContract() {}

public:
    const static std::string \
        ShaderNames_Prefix,
        Vertex_In_Position,
        Vertex_Out_Position,
        Fragment_In_Vertex_Position_Raw,
        Fragment_In_Vertex_Position_Model,
        Fragment_In_Vertex_Position_View,
        Fragment_In_Vertex_Position_Projection,
        Uniform_Position_Camera,
        Uniform_Matrix_Model,
        Uniform_Matrix_View,
        Uniform_Matrix_Projection,
        Filepath_Shader_Vertex_Pass,
        Filepath_Shader_Fragment_Pass,
        Filepath_Shader_Vertex_Model
    ;
};

#endif // SHADERCONTRACT_H
