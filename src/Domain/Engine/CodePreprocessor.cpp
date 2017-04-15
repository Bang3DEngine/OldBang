#include "Bang/CodePreprocessor.h"

#include <algorithm>

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/IO.h"

String CodePreprocessor::
       GetIncludeReplacementString(const String &includeDirective,
                                   int includeDirectiveLine,
                                   const List<String> &includePaths)
{
    Array<String> includeParts = includeDirective.Split('"', true);
    if (includeParts.Size() < 2) { return ""; }

    String includeFile = includeParts[1];

    // Search if the include file combined with any of the include paths
    // gives us an existing file. Both in Engine and in Users code
    String completeFilepath = "";
    for (String includePathDir : includePaths)
    {
        String filepath = "";
        includePathDir = IO::ToAbsolute(includePathDir, true);
        filepath = includePathDir + "/" + includeFile;
        if (!File::Exists(filepath))
        {
            includePathDir = IO::ToAbsolute(includePathDir, false);
        }
        filepath = includePathDir + "/" + includeFile;

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
        if (includeDirectiveLine > 0)
        {
            includeContents += "\n";
            includeContents += "#line " + String::ToString(includeDirectiveLine - 1);
            includeContents += "\n";
        }

        return includeContents;
    }
    else
    {
        //Debug_Error("Include file '" << includeFile << "' could not be found.");
    }

    return "";
}

void CodePreprocessor::PreprocessCode(String *srcCode,
                                      const List<String> &includePaths)
{
    String &code = *srcCode;

    Array<String> linesArray = code.Split('\n');
    List<String> lines = linesArray.ToList();

    // Keep track of the user's source line number, to use #line directive
    int originalLineNum = 1;

    for (auto it = lines.Begin(); it != lines.End(); ++it)
    {
        String line = (*it).Trim();
        if (line.BeginsWith("#include"))
        {
			lines.Remove(it++);

            String content = GetIncludeReplacementString(line, originalLineNum,
                                                         includePaths);

            // Get the include content lines, and add it to the overall lines.
            // This way we can process it recursively, since the included lines
            // are processed too
            List<String> contentLines = content.Split('\n').ToList();
            originalLineNum -= contentLines.Size(); // Do this before the splice

            lines.Splice(it, contentLines, contentLines.Begin(),
                         contentLines.End());
            it = lines.Begin();
        }

        ++originalLineNum;
    }

    code = String::Join(lines, "\n"); // Update the code adding all the lines
}


CodePreprocessor::CodePreprocessor()
{
}
