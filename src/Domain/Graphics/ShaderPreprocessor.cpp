#include "ShaderPreprocessor.h"

ShaderPreprocessor::ShaderPreprocessor()
{
}

void ShaderPreprocessor::PreprocessShaderCode(std::string *shaderSourceCode)
{
    std::string &code = *shaderSourceCode;

    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Declare + ";",
                         ShaderContract::Macro_Post_Render_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Init_Main + ";",
                         ShaderContract::Macro_Post_Render_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Declare + ";",
                         ShaderContract::Macro_Post_Render_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Init_Main + ";",
                         ShaderContract::Macro_Post_Render_FS_Init_Main_Content);

    // Same but without semicolon:
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Declare,
                         ShaderContract::Macro_Post_Render_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Init_Main,
                         ShaderContract::Macro_Post_Render_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Declare,
                         ShaderContract::Macro_Post_Render_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Init_Main,
                         ShaderContract::Macro_Post_Render_FS_Init_Main_Content);
}
