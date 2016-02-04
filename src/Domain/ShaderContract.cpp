#include "ShaderContract.h"

const std::string ShaderContract::ShaderNames_Prefix = "BANG_";



const std::string ShaderContract::Vertex_In_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_raw_vin";

const std::string ShaderContract::Vertex_In_Normal_Raw =
        ShaderContract::ShaderNames_Prefix + "normal_raw_vin";

const std::string ShaderContract::Vertex_In_Uv_Raw =
        ShaderContract::ShaderNames_Prefix + "uv_raw_vin";



const std::string ShaderContract::Vertex_Out_Fragment_In_Vertex_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_raw_vout_fin";

const std::string ShaderContract::Vertex_Out_Fragment_In_Vertex_Position_Model =
        ShaderContract::ShaderNames_Prefix + "position_model_vout_fin";

const std::string ShaderContract::Vertex_Out_Fragment_In_Vertex_Position_View =
        ShaderContract::ShaderNames_Prefix + "position_view_vout_fin";

const std::string ShaderContract::Vertex_Out_Fragment_In_Vertex_Position_Projection =
        ShaderContract::ShaderNames_Prefix + "position_projection_vout_fin";

const std::string ShaderContract::Vertex_Out_Fragment_In_Vertex_Normal_Raw =
        ShaderContract::ShaderNames_Prefix + "normal_raw_vout_fin";

const std::string ShaderContract::Vertex_Out_Fragment_In_Vertex_Uv_Raw =
        ShaderContract::ShaderNames_Prefix + "uv_raw_vout_fin";



const std::string ShaderContract::Uniform_Position_Camera =
        ShaderContract::ShaderNames_Prefix + "position_camera";

const std::string ShaderContract::Uniform_Matrix_Model =
        ShaderContract::ShaderNames_Prefix + "matrix_model";

const std::string ShaderContract::Uniform_Matrix_View =
        ShaderContract::ShaderNames_Prefix + "matrix_view";

const std::string ShaderContract::Uniform_Matrix_Projection =
        ShaderContract::ShaderNames_Prefix + "matrix_projection";



const std::string ShaderContract::Filepath_Shader_Vertex_Pass_Position   = "res/Shaders/pass_pos.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_Pass_Position_Normal = "res/Shaders/pass_pos_normal.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_Model_Position  = "res/Shaders/model_pos.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_Model_Position_Normal  = "res/Shaders/model_pos_normal.vert";

const std::string ShaderContract::Filepath_Shader_Fragment_Pass_Position  = "res/Shaders/pass_pos.frag";
const std::string ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal  = "res/Shaders/pass_pos_normal.frag";

