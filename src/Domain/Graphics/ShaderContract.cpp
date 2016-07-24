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

const std::string ShaderContract::Uniform_Matrix_Normal =
        ShaderContract::ShaderNames_Prefix + "matrix_normal";
const std::string ShaderContract::Uniform_Matrix_Normal_Inverse =
        ShaderContract::ShaderNames_Prefix + "matrix_normal_inv";

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
uniform mat4  B_matrix_model_inv; \n\
uniform mat4  B_matrix_normal; \n\
uniform mat4  B_matrix_normal_inv; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_view_inv; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_projection_inv; \n\
uniform mat4  B_matrix_pvm; \n\
uniform mat4  B_matrix_pvm_inv; \n\
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
B_vout.normal_world   = B_matrix_normal * vec4(B_vin.normal_object,   0.0); \n\
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
"\n\
#version 130 \n\
\n\
uniform mat4  B_matrix_model; \n\
uniform mat4  B_matrix_model_inv; \n\
uniform mat4  B_matrix_normal; \n\
uniform mat4  B_matrix_normal_inv; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_view_inv; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_projection_inv; \n\
uniform mat4  B_matrix_pvm; \n\
\n\
uniform vec3  B_position_camera; \n\
uniform vec4  B_material_diffuse_color; \n\
uniform float B_renderer_receivesLighting; \n\
uniform float B_gameObject_isSelected; \n\
\n\
in vec4 B_position_world_vout_fin; \n\
in vec4 B_normal_world_vout_fin; \n\
in vec2 B_uv_vout_fin; \n\
\n\
out vec4 B_position_fout_gin; \n\
out vec4 B_normal_fout_gin; \n\
out vec4 B_uv_fout_gin; \n\
out vec4 B_diffuse_fout_gin; \n\
out vec4 B_materialBools_fout_gin; \n\
out vec4 B_depth_fout_gin; \n\
out vec4 B_color_fout_gin; \n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content =
"\n\
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
B_vout.diffuseColor     = B_material_diffuse_color.rgb; \n\
B_vout.depth            = gl_FragCoord.z;  \n\
B_materialBools_fout_gin.w = 0.0f; \n\
\n\
";

const std::string ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main_Content =
"\n\
B_position_fout_gin       = vec4(B_vout.position_world, 1); \n\
B_normal_fout_gin         = vec4(B_vout.normal_world, 0); \n\
B_uv_fout_gin             = vec4(B_vout.uv, 0, 0); \n\
B_diffuse_fout_gin        = vec4(B_vout.diffuseColor.rgb, 1); \n\
B_materialBools_fout_gin  = vec4(B_vout.receivesLighting, 0, 0, B_materialBools_fout_gin.w); \n\
B_depth_fout_gin          = vec4(B_vout.depth); \n\
\n\
float ambientLight = (B_vout.receivesLighting > 0.5f) ? 0.5f : 1.0f; \n\
B_color_fout_gin   = vec4(B_vout.diffuseColor.rgb * ambientLight, 1);  \n\
";


const std::string ShaderContract::Macro_Post_Render_VS_Declare_Content =
"\
#version 130 \n\
\n\
uniform mat4  B_matrix_model; \n\
uniform mat4  B_matrix_model_inv; \n\
uniform mat4  B_matrix_normal; \n\
uniform mat4  B_matrix_normal_inv; \n\
uniform mat4  B_matrix_view; \n\
uniform mat4  B_matrix_view_inv; \n\
uniform mat4  B_matrix_projection; \n\
uniform mat4  B_matrix_projection_inv; \n\
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
uniform mat4 B_matrix_view; \n\
uniform mat4 B_matrix_view_inv; \n\
uniform mat4 B_matrix_projection; \n\
uniform mat4 B_matrix_projection_inv; \n\
uniform mat4 B_matrix_pvm; \n\
\n\
uniform vec2 B_screen_size; \n\
vec2 B_screen_coord = gl_FragCoord.xy; \n\
vec2 B_screen_coord_norm = B_screen_coord / B_screen_size; \n\
vec2 B_pixel_step = 1.0f / B_screen_size; \n\
\n\
uniform sampler2D B_position_gout_fin; \n\
uniform sampler2D B_normal_gout_fin; \n\
uniform sampler2D B_uv_gout_fin; \n\
uniform sampler2D B_diffuse_gout_fin;\n\
uniform sampler2D B_materialBools_gout_fin; \n\
uniform sampler2D B_depth_gout_fin; \n\
uniform sampler2D B_color_gout_fin; \n\
\n\
in vec3 B_position_raw_vin; \n\
in vec2 B_uv_raw_vout_fin; \n\
\n\
out vec4 B_color_gout_gin; /*Accumulated color*/ \n\
\n\
";

const std::string ShaderContract::Macro_Post_Render_FS_Init_Main_Content =
"\
B_vin.uv_screen           = B_uv_raw_vout_fin; \n\
B_vin.position_world      = texture2D(B_position_gout_fin,       B_vin.uv_screen).xyz;  \n\
B_vin.normal_world        = texture2D(B_normal_gout_fin,         B_vin.uv_screen).xyz;  \n\
B_vin.uv                  = texture2D(B_uv_gout_fin,             B_vin.uv_screen).xy;   \n\
B_vin.diffuseColor        = texture2D(B_diffuse_gout_fin,        B_vin.uv_screen).rgb;  \n\
B_vin.receivesLighting    = texture2D(B_materialBools_gout_fin,  B_vin.uv_screen).x;    \n\
B_vin.depth               = texture2D(B_depth_gout_fin,          B_vin.uv_screen).x;    \n\
B_vin.color               = texture2D(B_color_gout_fin,          B_vin.uv_screen).rgba; \n\
B_vin.normal_world        = normalize(B_vin.normal_world); \n\
\n\
/*Default value*/\n\
B_vout.color              = vec4(B_vin.color.rgb, 1); \n\
";

const std::string ShaderContract::Macro_Post_Render_FS_End_Main_Content =
"\
B_color_gout_gin = B_vout.color; \n\
";

const std::string ShaderContract::Filepath_Shader_SelectionBuffer_VS = "Assets/Engine/Shaders/SelectionBuffer.vert";
const std::string ShaderContract::Filepath_Shader_SelectionBuffer_FS = "Assets/Engine/Shaders/SelectionBuffer.frag";

const std::string ShaderContract::Filepath_Shader_Render_GBuffer_To_Screen_FS = "Assets/Engine/Shaders/RenderGBufferToScreen.frag";

const std::string ShaderContract::Filepath_Shader_PR_DrawScreenPlane_VS = "Assets/Engine/Shaders/PR_DrawScreenPlane.vert";
const std::string ShaderContract::Filepath_Shader_PR_Default_FS = "Assets/Engine/Shaders/PR_Default.frag";


const std::string ShaderContract::Filepath_Shader_D2G_Default_VS = "Assets/Engine/Shaders/D2G_Default.vert";
const std::string ShaderContract::Filepath_Shader_D2G_Default_FS  = "Assets/Engine/Shaders/D2G_Default.frag";

