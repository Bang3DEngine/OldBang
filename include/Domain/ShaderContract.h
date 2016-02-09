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

        Vertex_In_Position_Raw,
        Vertex_In_Normal_Raw,
        Vertex_In_Uv_Raw,

        Vertex_Out_Fragment_In_Vertex_Position_Raw,
        Vertex_Out_Fragment_In_Vertex_Position_Model,
        Vertex_Out_Fragment_In_Vertex_Position_View,
        Vertex_Out_Fragment_In_Vertex_Position_Projection,
        Vertex_Out_Fragment_In_Vertex_Normal_Raw,
        Vertex_Out_Fragment_In_Vertex_Uv_Raw,

        Uniform_Position_Camera,
        Uniform_Matrix_Model,
        Uniform_Matrix_View,
        Uniform_Matrix_Projection,
        Uniform_Matrix_PVM,

        Filepath_Shader_Vertex_Pass_Position,
        Filepath_Shader_Vertex_Pass_Position_Normal,
        Filepath_Shader_Vertex_Model_Position,
        Filepath_Shader_Vertex_Model_Position_Normal,
        Filepath_Shader_Vertex_PVM_Position_Normal,
        Filepath_Shader_Fragment_Pass_Position,
        Filepath_Shader_Fragment_Pass_Position_Normal
    ;
};

#endif // SHADERCONTRACT_H
