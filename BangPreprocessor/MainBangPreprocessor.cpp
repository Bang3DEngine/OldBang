

#include "Bang/File.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/BPReflectedStruct.h"
#include "Bang/BPReflectedVariable.h"

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        std::cerr << "Please specify as first parameter the file path"
                  << std::endl;
        return 1;
    }

    Path filepath(argv[1]);
    File file(filepath);

    String srcContents = file.GetContents();
    String reflHeaderContents;
    bool preprocessedSomething;
    BangPreprocessor::Preprocess(srcContents,
                                 &reflHeaderContents,
                                 &preprocessedSomething);
    if (preprocessedSomething)
    {
        String originalExt = filepath.GetExtension();
        Path reflFilepath = filepath.GetDirectory()
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
            std::cout << "  File '" << filepath.ToString().ToCString()
                      << "' successfully preprocessed into '"
                      << reflFilepath.ToString().ToCString()
                      << "'" << std::endl;
        }
    }
}
