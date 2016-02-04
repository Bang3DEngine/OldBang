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

        ShaderNames_Prefix = "BANG_",

        Vertex_In_Position  = ShaderNamePrefix + "position_vin",
        Vertex_Out_Position = ShaderNamePrefix + "position_vout_fin",

        Fragment_In_Vertex_Position_Raw        = ShaderNamePrefix + "position_raw_vout_fin",
        Fragment_In_Vertex_Position_Model      = ShaderNamePrefix + "position_model_vout_fin",
        Fragment_In_Vertex_Position_View       = ShaderNamePrefix + "position_view_vout_fin",
        Fragment_In_Vertex_Position_Projection = ShaderNamePrefix + "position_projection_vout_fin",

        Uniform_Position_Camera = ShaderNamePrefix + "position_camera",

        Uniform_Matrix_Model      = ShaderNamePrefix + "matrix_model",
        Uniform_Matrix_View       = ShaderNamePrefix + "matrix_view",
        Uniform_Matrix_Projection = ShaderNamePrefix + "matrix_projection"
    ;

    const static std::string \

        Filepath_Shader_Pass_Vertex   = "res/Shaders/pass.vert",
        Filepath_Shader_Pass_Fragment = "res/Shaders/pass.frag",
        Filepath_Shader_Model_Vertex  = "res/Shaders/model.vert"
    ;

};

#endif // SHADERNAMING_H
