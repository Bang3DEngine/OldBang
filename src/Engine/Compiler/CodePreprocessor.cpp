#include "Bang/CodePreprocessor.h"

#include <algorithm>

#include "Bang/Path.h"
#include "Bang/List.h"
#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Array.h"
#include "Bang/String.h"

USING_NAMESPACE_BANG

String
CodePreprocessor::GetIncludeContents(const String &includeDirective,
                                     const List<Path> &includePaths)
{
    Array<String> includeParts = includeDirective.Split<Array>('"', true);
    if (includeParts.Size() < 2) { return ""; }

    Path includeFile(includeParts[1]);

    // Search if the include file combined with any of the include paths
    // gives us an existing file. Both in Engine and in Users code
    Path completeFilepath;
    for (const Path& includeDirPath : includePaths)
    {
        completeFilepath = includeDirPath.Append(includeFile);
        if (completeFilepath.IsFile()) { break; }
    }

    String includeContents = "";
    if (completeFilepath.IsFile())
    {
        includeContents = File::GetContents(completeFilepath);
    }
    else
    {
        Debug_Error("Include file '" << includeFile << "' could not be found.");
    }

    return includeContents;
}

void CodePreprocessor::PreprocessCode(String *srcCode,
                                      const List<Path> &includePaths)
{
    String &outputCode = *srcCode;

    Array<String> linesArray = outputCode.Split<Array>('\n');
    List<String> lines = linesArray.To<List>();

    // Keep track of the user's source line number, to use #line directive
    int originalLineNum = 1;
    for (auto it = lines.Begin(); it != lines.End(); )
    {
        String line = (*it).Trim();
        if (line.BeginsWith("#include"))
        {
            it = lines.Remove(it);
            String includeContent = GetIncludeContents(line, includePaths);
            CodePreprocessor::PreprocessCode(&includeContent, includePaths);

            List<String> contentLines = includeContent.Split<List>('\n', false);
            contentLines.PushBack("#line " + String(originalLineNum-1));
            lines.Splice(it,
                         contentLines,
                         contentLines.Begin(),
                         contentLines.End());
            ::std::advance(it, contentLines.Size());
        }
        else { ++it; }

        ++originalLineNum;
    }

    if (lines.Front().BeginsWith("#version")) { lines.Insert(1, "#line 1\n"); }
    else { lines.Insert(0, "#line 0\n"); }

    outputCode = String::Join(lines, "\n");
}


CodePreprocessor::CodePreprocessor()
{
}
