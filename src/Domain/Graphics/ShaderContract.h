#ifndef SHADERCONTRACT_H
#define SHADERCONTRACT_H

#include "Bang.h"

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

        Fragment_Out_GBuffer_In_Position,
        Fragment_Out_GBuffer_In_Normal,
        Fragment_Out_GBuffer_In_Uv,
        Fragment_Out_GBuffer_In_Diffuse,

        Uniform_Position_Camera,

        Uniform_Matrix_Model,
        Uniform_Matrix_View,
        Uniform_Matrix_Projection,
        Uniform_Matrix_PVM,

        Uniform_Texture_Prefix,

        // When rendering to screen after Deferred Rendering passes.
        // This texture will hold the definitive color values of the rendered stage
        Uniform_Texture_Final_Color,
        Uniform_Material_Diffuse_Color,

        Uniform_Selected_In_Editor,

        Filepath_Shader_Vertex_Render_To_Screen,
        Filepath_Shader_Fragment_Render_To_Screen,

        Filepath_Shader_Vertex_Pass_Position,
        Filepath_Shader_Vertex_Pass_Position_Normal,
        Filepath_Shader_Vertex_Model_Position,
        Filepath_Shader_Vertex_Model_Position_Normal,
        Filepath_Shader_Vertex_PVM_Position_Normal,
        Filepath_Shader_Vertex_PVM_Position_Normal_Uv,

        Filepath_Shader_Fragment_Pass_Position,
        Filepath_Shader_Fragment_Pass_Position_Normal,
        Filepath_Shader_Fragment_Pass_Position_Normal_Uv
    ;
};

#endif // SHADERCONTRACT_H
