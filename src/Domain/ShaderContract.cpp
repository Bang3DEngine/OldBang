#include "ShaderContract.h"

const std::string ShaderContract::ShaderNames_Prefix = "BANG_";

const std::string ShaderContract::Vertex_In_Position =
        ShaderContract::ShaderNames_Prefix + "position_vin";

const std::string ShaderContract::Vertex_Out_Position =
        ShaderContract::ShaderNames_Prefix + "position_vout_fin";

const std::string ShaderContract::Fragment_In_Vertex_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_raw_vout_fin";

const std::string ShaderContract::Fragment_In_Vertex_Position_Model =
        ShaderContract::ShaderNames_Prefix + "position_model_vout_fin";

const std::string ShaderContract::Fragment_In_Vertex_Position_View =
        ShaderContract::ShaderNames_Prefix + "position_view_vout_fin";

const std::string ShaderContract::Fragment_In_Vertex_Position_Projection =
        ShaderContract::ShaderNames_Prefix + "position_projection_vout_fin";

const std::string ShaderContract::Uniform_Position_Camera =
        ShaderContract::ShaderNames_Prefix + "position_camera";

const std::string ShaderContract::Uniform_Matrix_Model =
        ShaderContract::ShaderNames_Prefix + "matrix_model";

const std::string ShaderContract::Uniform_Matrix_View =
        ShaderContract::ShaderNames_Prefix + "matrix_view";

const std::string ShaderContract::Uniform_Matrix_Projection =
        ShaderContract::ShaderNames_Prefix + "matrix_projection";


const std::string ShaderContract::Filepath_Shader_Vertex_Pass   = "res/Shaders/pass.vert";
const std::string ShaderContract::Filepath_Shader_Fragment_Pass = "res/Shaders/pass.frag";
const std::string ShaderContract::Filepath_Shader_Vertex_Model  = "res/Shaders/model.vert";

