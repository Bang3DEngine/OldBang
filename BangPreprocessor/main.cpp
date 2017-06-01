#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/BPStruct.h"
#include "Bang/BPProperty.h"
#include "Bang/SystemUtils.h"
#include "Bang/BangPreprocessor.h"

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

    String contents = file.GetContents();
    contents = BangPreprocessor::Preprocess(contents);

    String originalExt = filepath.GetExtension();
    filepath = filepath.GetDirectory().Append(filepath.GetName())
                                      .AppendExtension("gen")
                                      .AppendExtension(originalExt);
    Debug_Log(filepath);
    File::Write(Path(filepath), contents);
}
