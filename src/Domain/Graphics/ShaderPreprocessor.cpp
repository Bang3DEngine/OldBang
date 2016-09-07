#include "ShaderPreprocessor.h"

#include "Debug.h"

const Array<String> ShaderPreprocessor::c_includePaths =
{
    "Assets/Engine/Shaders/Include"
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

        // Using #line, error lines info will be reported as in the original file,
        // for a more user-friendly debugging
        includeContents += "\n";
        includeContents += "#line " + String::ToString(includeDirectiveLine);
        includeContents += "\n";

        return includeContents;
    }
    else
    {
        Debug_Error("Include file '" << includeFile << "' could not found.");
    }

    return "";
}

// TODO: Do recursively. Solve #line problem
void ShaderPreprocessor::PreprocessShaderCode(String *shaderSourceCode)
{
    String &code = *shaderSourceCode;

    Array<String> linesArray = code.Split('\n');
    List<String> lines = linesArray.ToList();

    // Keep track of the user's source line number, to use #line directive
    int originalLineNum = 0;

    // It supports recursive #include's.
    for (auto it = lines.Begin(); it != lines.End(); ++it)
    {
        String line = (*it).Trimmed();
        if (line.BeginsWith("#include"))
        {
            it = lines.Remove(it);

            --it;
            List<String>::Iterator beforeInsertingContent = it;
            ++it;

            String content = GetIncludeReplacementString(line, originalLineNum + 1);

            // Get the include content lines, and add it to the overall lines.
            // This way we can process it recursively, since the included lines
            // are processed too
            List<String> contentLines = content.Split('\n').ToList();
            lines.Splice(it, contentLines, contentLines.Begin(), contentLines.End());

            originalLineNum -= contentLines.Size();
            it = beforeInsertingContent;
        }
        ++originalLineNum;
    }

    code = String::Join(lines, "\n"); // Update the code String adding all the lines
}
