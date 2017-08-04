#include "BangPreprocessor.h"

#include <QProcess>

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/Array.h"
#include "Bang/BPReflectedStruct.h"
#include "Bang/BPReflectedVariable.h"

using BP = BangPreprocessor;

const Array<String> BP::VarTypeInt     = {"int"};
const Array<String> BP::VarTypeBool    = {"bool"};
const Array<String> BP::VarTypeColor   = {"Color"};
const Array<String> BP::VarTypeFloat   = {"float"};
const Array<String> BP::VarTypeDouble  = {"double"};
const Array<String> BP::VarTypeString  = {"std::string", "string", "String"};
const Array<String> BP::VarTypeVector2 = {"Vector2"};
const Array<String> BP::VarTypeVector3 = {"Vector3"};
const Array<String> BP::VarTypeVector4 = {"Vector4"};
const Array<String> BP::VarTypeQuaternion = {"Quaternion"};

const Array<String> BP::Modifiers =
{
    "const",
    "constexpr",
    "volatile",
    "static"
};

const Array<String> BP::VarTypes =
{
    BP::VarTypeInt[0],
    BP::VarTypeBool[0],
    BP::VarTypeColor[0],
    BP::VarTypeFloat[0],
    BP::VarTypeDouble[0],
    BP::VarTypeString[0], BP::VarTypeString[1], BP::VarTypeString[2],
    BP::VarTypeVector2[0],
    BP::VarTypeVector3[0],
    BP::VarTypeVector4[0],
    BP::VarTypeQuaternion[0],
    "struct",
    "class"
};

const Array<String> BP::RVariablePrefixes =
{
    "BP_REFLECT_VARIABLE"
};

const Array<String> BP::RStructPrefixes =
{
    "BP_REFLECT_CLASS",
    "BP_REFLECT_STRUCT"
};

const String BP::ReflectDefinitionsDefineName = "BP_REFLECT_DEFINITIONS";
const String BP::ReflectionInfoVarName = "BP_ReflectionInfo";

void BangPreprocessor::Preprocess(const Path &filepath)
{
    File file(filepath);
    String srcContents = file.GetContents();
    String reflHeaderContents;
    bool preprocessedSomething;
    BangPreprocessor::Preprocess(srcContents,
                                 &reflHeaderContents,
                                 &preprocessedSomething);

    String originalExt = filepath.GetExtension();
    Path reflFilepath  = filepath.GetDirectory()
                                 .Append("." + filepath.GetName())
                                 .AppendExtension("refl")
                                 .AppendExtension(originalExt);

    bool writePreprocessedFile = true;
    if (reflFilepath.Exists())
    {
        String oldReflContents = File(reflFilepath).GetContents();
        writePreprocessedFile = (oldReflContents != reflHeaderContents);
    }

    if (writePreprocessedFile)
    {
        File::Write(reflFilepath, reflHeaderContents);

        if (preprocessedSomething)
        {
            std::cout << "  File '" << filepath.ToString().ToCString()
                      << "' successfully preprocessed into '"
                      << reflFilepath.ToString().ToCString()
                      << "'" << std::endl;
        }
    }
}

void BangPreprocessor::Preprocess(const String &source,
                                  String *_reflectionHeaderSource,
                                  bool *preprocessedSomething)
{
    *preprocessedSomething = false;
    String &reflectionHeaderSource = *_reflectionHeaderSource;
    reflectionHeaderSource = R"VERBATIM(
             #include "Bang/XMLNode.h"
             #include "Bang/BPReflectedStruct.h"
             #include "Bang/BPReflectedVariable.h"
          )VERBATIM";

    String src = source;
    BP::RemoveComments(&src);
    String::Iterator it = src.Begin();
    while (true)
    {
        // Find Structure/Class annotation
        String::Iterator itStructBegin = BP::Find(it, src.End(),
                                                  BP::RStructPrefixes);
        if (itStructBegin == src.End()) { break; }

        String::Iterator itStructScopeBegin, itStructScopeEnd;
        BP::GetNextScope(itStructBegin,
                         src.End(),
                         &itStructScopeBegin,
                         &itStructScopeEnd,
                         '{',
                         '}');
        it = itStructScopeEnd;
        if (itStructScopeBegin == src.End()) { break; }

        bool ok;
        BPReflectedStruct reflStruct;
        BPReflectedStruct::FromString(itStructBegin, itStructScopeEnd,
                                      &reflStruct, &ok);

        String reflectDefineCode =
                "#define  REFLECT_DEFINITIONS_DEFINE_NAME_RSTRUCT_VAR_NAME() ";
        reflectDefineCode += R"VERBATIM( public:
                    GET_REFLECTION_INFO_CODE
                    GET_READ_REFLECTION_CODE
                    GET_WRITE_REFLECTION_CODE
                private:   )VERBATIM";
        reflectDefineCode.ReplaceInSitu("GET_REFLECTION_INFO_CODE",
                                        reflStruct.GetGetReflectionInfoCode());
        reflectDefineCode.ReplaceInSitu("GET_READ_REFLECTION_CODE",
                                        reflStruct.GetReadReflectionCode());
        reflectDefineCode.ReplaceInSitu("GET_WRITE_REFLECTION_CODE",
                                        reflStruct.GetWriteReflectionCode());
        reflectDefineCode.ReplaceInSitu("\n", "\\\n");

        reflectDefineCode += "\n"
                "#undef REFLECT_DEFINITIONS_DEFINE_NAME \n"
                "#define REFLECT_DEFINITIONS_DEFINE_NAME(ClassName) \
                         REFLECT_DEFINITIONS_DEFINE_NAME_##ClassName()";

        reflectDefineCode.ReplaceInSitu("RSTRUCT_VAR_NAME",
                                        reflStruct.GetStructVariableName());
        reflectDefineCode.ReplaceInSitu("REFLECT_DEFINITIONS_DEFINE_NAME",
                                        BP::ReflectDefinitionsDefineName);

        reflectionHeaderSource += reflectDefineCode;
        *preprocessedSomething = true;
    }
}

void BangPreprocessor::RemoveComments(String *source)
{
    String &src = *source;

    QProcess gCompilerProcess;
    gCompilerProcess.setReadChannel(QProcess::ProcessChannel::StandardOutput);
    gCompilerProcess.start("g++", {"-fpreprocessed", "-E", "-"});

    gCompilerProcess.write( src.ToCString(), src.Length() );
    gCompilerProcess.closeWriteChannel();

    bool ok = gCompilerProcess.waitForFinished(999999);
    ok = ok && (gCompilerProcess.exitCode() == 0);

    String output = String(
                       QString( gCompilerProcess.readAllStandardOutput() )
                    );
    gCompilerProcess.close();

    output.Erase(output.Begin(), output.Find('\n')+1); // Erase first line
    *source = output;
}

String::Iterator BangPreprocessor::Find(String::Iterator begin,
                                        String::Iterator end,
                                        const Array<String> &toFindList)
{
    for (const String &toFind : toFindList)
    {
        String::Iterator itFound = std::search(begin, end,
                                               toFind.Begin(), toFind.End());
        if (itFound != end) { return itFound; }
    }
    return end;
}

void BangPreprocessor::GetNextScope(String::Iterator begin,
                                    String::Iterator end,
                                    String::Iterator *scopeBegin,
                                    String::Iterator *scopeEnd,
                                    char openingBrace,
                                    char closingBrace)
{
    int insideness = 0;
    String::Iterator it = begin;
    for (; it != end; ++it)
    {
        if (*it == openingBrace)
        {
            if (insideness == 0) { *scopeBegin = it; }
            ++insideness;
        }
        else if (*it == closingBrace)
        {
            --insideness;
            if (insideness == 0)
            {
                *scopeEnd = it + 1;
                break;
            }
        }
    }

    if (insideness != 0) { *scopeBegin = *scopeEnd = end; }
}

void BangPreprocessor::SkipBlanks(String::Iterator *it,
                                  String::Iterator end)
{
    char c = *(*it);
    while (c == '\n' || c == '\r' || c == '\t' || c == ' ')
    {
        ++(*it);
        if (*it == end) { break; }
        c = *(*it);
    }
}

void BangPreprocessor::SkipUntilNextBlank(String::Iterator *it,
                                          String::Iterator end)
{
    SkipUntilNext(it, end, {"\n", "\r", "\t", " "});
}

void BangPreprocessor::SkipUntilNext(String::Iterator *it,
                                     String::Iterator end,
                                     const Array<String> &particles)
{
    String c( std::string(1, *(*it)) );
    while ( !particles.Contains(c) )
    {
        ++(*it);
        if (*it == end) { break; }
        c = String( std::string(1, *(*it)) );
    }
}

void BangPreprocessor::FindNextWord(String::Iterator begin,
                                    String::Iterator end,
                                    String::Iterator *wordBegin,
                                    String::Iterator *wordEnd)
{
    *wordBegin = begin;
    BP::SkipBlanks(wordBegin, end);

    *wordEnd = (*wordBegin + 1);
    char c = *(*wordEnd);
    while (String::IsLetter(c) || String::IsNumber(c) || c == '_')
    {
        ++(*wordEnd);
        if (*wordEnd == end) { break; }
        c = *(*wordEnd);
    }
}

BangPreprocessor::BangPreprocessor()
{
}
