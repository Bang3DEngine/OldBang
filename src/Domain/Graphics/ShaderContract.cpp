#include "ShaderContract.h"

const std::string ShaderContract::ShaderNames_Prefix = "B_";


const std::string ShaderContract::Attr_Vertex_In_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_raw_vin";

const std::string ShaderContract::Attr_Vertex_In_Normal_Raw =
        ShaderContract::ShaderNames_Prefix + "normal_raw_vin";

const std::string ShaderContract::Attr_Vertex_In_Uv_Raw =
        ShaderContract::ShaderNames_Prefix + "uv_raw_vin";



const std::string ShaderContract::Attr_Vertex_Out_Fragment_In_Vertex_Position_Raw =
        ShaderContract::ShaderNames_Prefix + "position_world_vout_fin";

const std::string ShaderContract::Attr_Vertex_Out_Fragment_In_Vertex_Normal_Raw =
        ShaderContract::ShaderNames_Prefix + "normal_world_vout_fin";

const std::string ShaderContract::Attr_Vertex_Out_Fragment_In_Vertex_Uv_Raw =
        ShaderContract::ShaderNames_Prefix + "uv_vout_fin";

const std::string ShaderContract::Attr_Fragment_Out_GBuffer_In_Position =
        ShaderContract::ShaderNames_Prefix + "position_fout_gin";

const std::string ShaderContract::Attr_Fragment_Out_GBuffer_In_Normal =
        ShaderContract::ShaderNames_Prefix + "normal_fout_gin";

const std::string ShaderContract::Attr_Fragment_Out_GBuffer_In_Uv =
        ShaderContract::ShaderNames_Prefix + "uv_fout_gin";

const std::string ShaderContract::Attr_Fragment_Out_GBuffer_In_Diffuse =
        ShaderContract::ShaderNames_Prefix + "diffuse_fout_gin";


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
const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_End_Main =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_VS_END_MAIN()";
const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Declare =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_FS_DECLARE()";
const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_FS_INIT_MAIN()";
const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main =
        ShaderContract::ShaderNames_Prefix + "DRAW_TO_GBUFFER_FS_END_MAIN()";
const std::string ShaderContract::Macro_Post_Render_VS_Declare =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_VS_DECLARE()";
const std::string ShaderContract::Macro_Post_Render_VS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_VS_INIT_MAIN()";
const std::string ShaderContract::Macro_Post_Render_VS_End_Main =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_VS_END_MAIN()";
const std::string ShaderContract::Macro_Post_Render_FS_Declare =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_FS_DECLARE()";
const std::string ShaderContract::Macro_Post_Render_FS_Init_Main =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_FS_INIT_MAIN()";
const std::string ShaderContract::Macro_Post_Render_FS_End_Main =
        ShaderContract::ShaderNames_Prefix + "POST_RENDER_FS_END_MAIN()";

const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_Declare_Content =
"\
#version 130 \n\
\n\
uniform mat4  B_matrix_model; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_pvm; \n\
in vec3 B_position_raw_vin; \n\
in vec3 B_normal_raw_vin; \n\
in vec2 B_uv_raw_vin; \n\
\n\
out vec4 B_position_world_vout_fin; \n\
out vec4 B_normal_world_vout_fin; \n\
out vec2 B_uv_vout_fin; \n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main_Content =
"\
/* Fill vin values */ \n\
B_vin.position_object = B_position_raw_vin; \n\
B_vin.normal_object   = B_normal_raw_vin; \n\
B_vin.uv              = B_uv_raw_vin; \n\
 \n\
/* Some default values */ \n\
B_vout.position_world = B_matrix_model * vec4(B_vin.position_object, 1.0); \n\
B_vout.normal_world   = B_matrix_model * vec4(B_vin.normal_object,   0.0); \n\
B_vout.uv             = vec2(B_vin.uv.x, 1.0f - B_vin.uv.y); \n\
B_vout.position_pvm   = B_matrix_pvm * vec4(B_vin.position_object,   1.0); \n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_VS_End_Main_Content =
"\
/* Pass the out values to the out attributes */ \n\
B_position_world_vout_fin = B_vout.position_world; \n\
B_normal_world_vout_fin   = B_vout.normal_world; \n\
B_uv_vout_fin             = B_vout.uv; \n\
gl_Position               = B_vout.position_pvm; \n\
";


const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Declare_Content =
"\
#version 130 \n\
\n\
uniform mat4  B_matrix_model; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_pvm; \n\
uniform vec3  B_position_camera; \n\
uniform vec4  B_material_diffuse_color; \n\
uniform float B_renderer_receivesLighting; \n\
\n\
in vec4 B_position_world_vout_fin; \n\
in vec4 B_normal_world_vout_fin; \n\
in vec2 B_uv_vout_fin; \n\
\n\
out vec3  B_position_fout_gin; \n\
out vec3  B_normal_fout_gin; \n\
out vec2  B_uv_fout_gin; \n\
out vec3  B_diffuse_fout_gin; \n\
out vec3  B_materialBools_fout_gin; \n\
out float B_depth_fout_gin; \n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content =
"\
/* Fill vin vin */ \n\
B_vin.position_world = B_position_world_vout_fin; \n\
B_vin.normal_world   = B_normal_world_vout_fin; \n\
B_vin.uv             = B_uv_vout_fin; \n\
 \n\
/* Some default values */ \n\
B_vout.position_world   = B_vin.position_world.xyz; \n\
B_vout.normal_world     = B_vin.normal_world.xyz; \n\
B_vout.uv               = B_vin.uv; \n\
B_vout.receivesLighting = B_renderer_receivesLighting; \n\
B_vout.depth            = gl_FragCoord.z;  \n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main_Content =
"\
B_position_fout_gin = B_vout.position_world; \n\
B_normal_fout_gin = B_vout.normal_world; \n\
B_uv_fout_gin = B_vout.uv; \n\
B_diffuse_fout_gin = B_vout.diffuseColor; \n\
B_materialBools_fout_gin.x = B_vout.receivesLighting; \n\
B_depth_fout_gin = B_vout.depth; \n\
";


const std::string ShaderContract::Macro_Post_Render_VS_Declare_Content =
"\
#version 130 \n\
\n\
uniform mat4  B_matrix_model; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_pvm; \n\
\n\
";

const std::string ShaderContract::Macro_Post_Render_VS_Init_Main_Content =
"\
\n\
";
const std::string ShaderContract::Macro_Post_Render_VS_End_Main_Content =
"\
\n\
";

const std::string ShaderContract::Macro_Post_Render_FS_Declare_Content =
"\
#version 130 \n\
\n\
uniform mat4  B_matrix_model; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_pvm; \n\
\n\
uniform sampler2D B_gout_fin_position; \n\
uniform sampler2D B_gout_fin_normal; \n\
uniform sampler2D B_gout_fin_uv; \n\
uniform sampler2D B_gout_fin_diffuse;\n\
uniform sampler2D B_gout_fin_materialBools; \n\
uniform sampler2D B_gout_fin_depth; \n\
\n\
in vec3 B_position_raw_vin; \n\
in vec2 B_uv_raw_vout_fin; \n\
vec2 B_screen_uv = B_uv_raw_vout_fin; \n\
\n\
";

const std::string ShaderContract::Macro_Post_Render_FS_Init_Main_Content =
"\
B_vin.position_world      = texture2D(B_gout_fin_position,       B_screen_uv).xyz; \n\
B_vin.normal_world        = texture2D(B_gout_fin_normal,         B_screen_uv).xyz; \n\
B_vin.uv                  = texture2D(B_gout_fin_uv,             B_screen_uv).xy;  \n\
B_vin.diffuseColor        = texture2D(B_gout_fin_diffuse,        B_screen_uv).rgb; \n\
B_vin.receivesLighting    = texture2D(B_gout_fin_materialBools,  B_screen_uv).x;   \n\
B_vin.depth               = texture2D(B_gout_fin_depth,          B_screen_uv).x;   \n\
B_vin.normal_world        = normalize(B_vin.normal_world); \n\
";
const std::string ShaderContract::Macro_Post_Render_FS_End_Main_Content =
"\
gl_FragColor = B_vout.color; \n\
";

const std::string ShaderContract::Filepath_Shader_SelectionBuffer_VS = "Assets/Engine/Shaders/SelectionBuffer.vert";
const std::string ShaderContract::Filepath_Shader_SelectionBuffer_FS = "Assets/Engine/Shaders/SelectionBuffer.frag";

const std::string ShaderContract::Filepath_Shader_PR_Default_VS = "Assets/Engine/Shaders/PR_Default.vert";
const std::string ShaderContract::Filepath_Shader_PR_Default_FS = "Assets/Engine/Shaders/PR_Default.frag";


const std::string ShaderContract::Filepath_Shader_D2G_Default_VS = "Assets/Engine/Shaders/D2G_Default.vert";
const std::string ShaderContract::Filepath_Shader_D2G_Default_FS  = "Assets/Engine/Shaders/D2G_Default.frag";

