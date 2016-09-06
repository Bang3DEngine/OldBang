#include "ShaderPreprocessor.h"

const Array<String> ShaderPreprocessor::c_includePaths =
{
    "Assets/Engine/Shaders/Includes"
};

ShaderPreprocessor::ShaderPreprocessor()
{
}

String ShaderPreprocessor::
       GetIncludeReplacementString(const String &includeDirective,
                                   int includeDirectiveLine)
{
    Array<String> includeParts = includeDirective.Split('"', true);
    if (includeParts.Size() < 3) { return ""; }

    String includeFile = includeParts[1];

    // Search if the include file combined with any of the include paths
    // gives us an existing file
    String completeFilepath = "";
    for (String includePathDir : c_includePaths)
    {
        String filepath = includePathDir + "/" + includeFile;
        if (File::Exists(filepath))
        {
            completeFilepath = filepath;
            break;
        }
    }

    if (!completeFilepath.Empty())
    {
        String includeContents = File::GetContents(completeFilepath);
        includeContents += "\n";

        // Using #line, error lines info will be reported as in the original file,
        // for a more user-friendly debugging
        includeContents += "#line " + includeDirectiveLine;

        return includeContents;
    }

    return "";
}

void ShaderPreprocessor::PreprocessShaderCode(String *shaderSourceCode)
{
    String &code = *shaderSourceCode;

    Array<String> linesArray = code.Split('\n');
    List<String> lines = linesArray.ToList();
    int i = 0;
    for (auto it = lines.Begin(); it != lines.End(); ++it)
    {
        String line = (*it).Trimmed();
        if (line.BeginsWith("#include"))
        {
            String content = GetIncludeReplacementString(line, i + 1);
            lines.InsertAfter(it, content);
        }
        ++i;
    }

    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_End_Main + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_VS_End_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main + ";",
                         ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Declare + ";",
                         ShaderContract::Macro_Post_Render_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Init_Main + ";",
                         ShaderContract::Macro_Post_Render_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_End_Main + ";",
                         ShaderContract::Macro_Post_Render_VS_End_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Declare + ";",
                         ShaderContract::Macro_Post_Render_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Init_Main + ";",
                         ShaderContract::Macro_Post_Render_FS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_End_Main + ";",
                         ShaderContract::Macro_Post_Render_FS_End_Main_Content);

    // Same but without semicolon:
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_End_Main,
                         ShaderContract::Macro_Draw_To_GBuffer_VS_End_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main,
                         ShaderContract::Macro_Draw_To_GBuffer_FS_End_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Declare,
                         ShaderContract::Macro_Post_Render_VS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_Init_Main,
                         ShaderContract::Macro_Post_Render_VS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_VS_End_Main,
                         ShaderContract::Macro_Post_Render_VS_End_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Declare,
                         ShaderContract::Macro_Post_Render_FS_Declare_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_Init_Main,
                         ShaderContract::Macro_Post_Render_FS_Init_Main_Content);
    StringUtils::Replace(&code,
                         ShaderContract::Macro_Post_Render_FS_End_Main,
                         ShaderContract::Macro_Post_Render_FS_End_Main_Content);
}
