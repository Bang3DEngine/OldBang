#include "ShaderContract.h"

const std::string ShaderContract::ShaderNames_Prefix = "B_";


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

const std::string ShaderContract::Fragment_Out_GBuffer_In_Position =
        ShaderContract::ShaderNames_Prefix + "position_fout_gbufferin";

const std::string ShaderContract::Fragment_Out_GBuffer_In_Normal =
        ShaderContract::ShaderNames_Prefix + "normal_fout_gbufferin";

const std::string ShaderContract::Fragment_Out_GBuffer_In_Uv =
        ShaderContract::ShaderNames_Prefix + "uv_fout_gbufferin";

const std::string ShaderContract::Fragment_Out_GBuffer_In_Diffuse =
        ShaderContract::ShaderNames_Prefix + "diffuse_fout_gbufferin";


const std::string ShaderContract::Uniform_Position_Camera =
        ShaderContract::ShaderNames_Prefix + "position_camera";

const std::string ShaderContract::Uniform_Matrix_Model =
        ShaderContract::ShaderNames_Prefix + "matrix_model";
const std::string ShaderContract::Uniform_Matrix_Model_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_model_inv";

const std::string ShaderContract::Uniform_Matrix_View =
        ShaderContract::ShaderNames_Prefix + "matrix_view";
const std::string ShaderContract::Uniform_Matrix_View_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_view_inv";

const std::string ShaderContract::Uniform_Matrix_Projection =
        ShaderContract::ShaderNames_Prefix + "matrix_projection";
const std::string ShaderContract::Uniform_Matrix_Projection_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_projection_inv";

const std::string ShaderContract::Uniform_Matrix_PVM =
        ShaderContract::ShaderNames_Prefix + "matrix_pvm";

const std::string ShaderContract::Uniform_Texture_Prefix =
        ShaderContract::ShaderNames_Prefix + "texture_";

const std::string ShaderContract::Uniform_Texture_Final_Color =
        ShaderContract::Uniform_Texture_Prefix + "final_color";

const std::string ShaderContract::Uniform_Material_Diffuse_Color =
        ShaderContract::ShaderNames_Prefix + "material_diffuse_color";







const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_Declare =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_VS_DECLARE()";
const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_VS_INIT_MAIN()";
const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Declare =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_FS_DECLARE()";
const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_FS_INIT_MAIN()";
const std::string ShaderContract::Macro_Post_Render_VS_Declare =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_VS_DECLARE()";
const std::string ShaderContract::Macro_Post_Render_VS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_VS_INIT_MAIN()";
const std::string ShaderContract::Macro_Post_Render_FS_Declare =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_FS_DECLARE()";
const std::string ShaderContract::Macro_Post_Render_FS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_FS_INIT_MAIN()";

const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_Declare_Content =
"\
\n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main_Content =
"\
\n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Declare_Content =
"\
\n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content =
"\
\n\
";

const std::string ShaderContract::Macro_Post_Render_VS_Declare_Content =
"\
\n\
";

const std::string ShaderContract::Macro_Post_Render_VS_Init_Main_Content =
"\
\n\
";
const std::string ShaderContract::Macro_Post_Render_FS_Declare_Content =
"\
#version 130 \n\
\n\
uniform sampler2D B_gout_fin_position; \n\
uniform sampler2D B_gout_fin_normal; \n\
uniform sampler2D B_gout_fin_uv; \n\
uniform sampler2D B_gout_fin_diffuse;\n\
uniform sampler2D B_gout_fin_material_bools; \n\
uniform sampler2D B_gout_fin_depth; \n\
\n\
in vec2 B_uv_raw_vout_fin; \n\
vec2 B_screen_uv = B_uv_raw_vout_fin; \n\
\n\
struct B_GVertex \n\
{ \n\
    vec3 pos; \n\
    vec3 normal; \n\
    vec2 uv; \n\
    vec3 diffColor; \n\
    float receivesLighting; \n\
    float depth; \n\
}; \n\
B_GVertex B_gVertex; \n\
";

const std::string ShaderContract::Macro_Post_Render_FS_Init_Main_Content =
"\
B_gVertex.pos              = texture2D(B_gout_fin_position,       B_screen_uv).xyz; \n\
B_gVertex.normal           = texture2D(B_gout_fin_normal,         B_screen_uv).xyz; \n\
B_gVertex.uv               = texture2D(B_gout_fin_uv,             B_screen_uv).xy;  \n\
B_gVertex.diffColor        = texture2D(B_gout_fin_diffuse,        B_screen_uv).rgb; \n\
B_gVertex.receivesLighting = texture2D(B_gout_fin_material_bools, B_screen_uv).x;   \n\
B_gVertex.depth            = texture2D(B_gout_fin_depth,          B_screen_uv).x;   \n\
B_gVertex.normal = normalize(B_gVertex.normal); \n\
";













const std::string ShaderContract::Filepath_Shader_Fragment_Selection = "Assets/Engine/Shaders/selection.frag";
const std::string ShaderContract::Filepath_Shader_Vertex_Render_To_Screen = "Assets/Engine/Shaders/render_to_screen.vert";
const std::string ShaderContract::Filepath_Shader_Fragment_Render_To_Screen = "Assets/Engine/Shaders/render_to_screen.frag";

const std::string ShaderContract::Filepath_Shader_Vertex_Pass_Position   = "Assets/Engine/Shaders/pass_pos.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_Pass_Position_Normal = "Assets/Engine/Shaders/pass_pos_normal.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_Model_Position  = "Assets/Engine/Shaders/model_pos.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_Model_Position_Normal  = "Assets/Engine/Shaders/model_pos_normal.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_PVM_Position  = "Assets/Engine/Shaders/pvm_pos.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal  = "Assets/Engine/Shaders/pvm_pos_normal.vert";
const std::string ShaderContract::Filepath_Shader_Vertex_PVM_Position_Normal_Uv = "Assets/Engine/Shaders/pvm_pos_normal_uv.vert";

const std::string ShaderContract::Filepath_Shader_Fragment_Pass_Position  = "Assets/Engine/Shaders/pass_pos.frag";
const std::string ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal  = "Assets/Engine/Shaders/pass_pos_normal.frag";
const std::string ShaderContract::Filepath_Shader_Fragment_Pass_Position_Normal_Uv  = "Assets/Engine/Shaders/pass_pos_normal_uv.frag";

