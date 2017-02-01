#include "ShaderContract.h"

#include "Persistence.h"

const String ShaderContract::ShaderNames_Prefix = "B_";


const String ShaderContract::Attr_Vertex_In_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_raw_vin";

const String ShaderContract::Attr_Vertex_In_Normal_Raw =
        ShaderContract::ShaderNames_Prefix + "normal_raw_vin";

const String ShaderContract::Attr_Vertex_In_Uv_Raw =
        ShaderContract::ShaderNames_Prefix + "uv_raw_vin";



const String ShaderContract::Attr_Vertex_Out_Fragment_In_Vertex_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_world_vout_fin";

const String ShaderContract::Attr_Vertex_Out_Fragment_In_Vertex_Normal_Raw =
        ShaderContract::ShaderNames_Prefix + "normal_world_vout_fin";

const String ShaderContract::Attr_Vertex_Out_Fragment_In_Vertex_Uv_Raw =
        ShaderContract::ShaderNames_Prefix + "uv_vout_fin";

const String ShaderContract::Attr_Fragment_Out_GBuffer_In_Position =
        ShaderContract::ShaderNames_Prefix + "position_fout_gin";

const String ShaderContract::Attr_Fragment_Out_GBuffer_In_Normal =
        ShaderContract::ShaderNames_Prefix + "normal_fout_gin";

const String ShaderContract::Attr_Fragment_Out_GBuffer_In_Uv =
        ShaderContract::ShaderNames_Prefix + "uv_fout_gin";

const String ShaderContract::Attr_Fragment_Out_GBuffer_In_Diffuse =
        ShaderContract::ShaderNames_Prefix + "diffuse_fout_gin";


const String ShaderContract::Uniform_Position_Camera =
        ShaderContract::ShaderNames_Prefix + "position_camera";

const String ShaderContract::Uniform_Matrix_Model =
        ShaderContract::ShaderNames_Prefix + "matrix_model";
const String ShaderContract::Uniform_Matrix_Model_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_model_inv";

const String ShaderContract::Uniform_Matrix_Normal =
        ShaderContract::ShaderNames_Prefix + "matrix_normal";
const String ShaderContract::Uniform_Matrix_Normal_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_normal_inv";

const String ShaderContract::Uniform_Matrix_View =
        ShaderContract::ShaderNames_Prefix + "matrix_view";
const String ShaderContract::Uniform_Matrix_View_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_view_inv";

const String ShaderContract::Uniform_Matrix_Projection =
        ShaderContract::ShaderNames_Prefix + "matrix_projection";
const String ShaderContract::Uniform_Matrix_Projection_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_projection_inv";

const String ShaderContract::Uniform_Matrix_PVM =
        ShaderContract::ShaderNames_Prefix + "matrix_pvm";

const String ShaderContract::Uniform_Texture_Prefix =
        ShaderContract::ShaderNames_Prefix + "texture_";

const String ShaderContract::Uniform_Texture_Final_Color =
        ShaderContract::Uniform_Texture_Prefix + "final_color";

const String ShaderContract::Uniform_Material_Diffuse_Color =
        ShaderContract::ShaderNames_Prefix + "material_diffuse_color";
