
#include "Bang/Path.h"
#include "Bang/BangPreprocessor.h"
#include "Bang/BPReflectedStruct.h"
#include "Bang/BPReflectedVariable.h"

USING_NAMESPACE_BANG

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        ::std::cerr << "Please specify as first parameter the file path"
                  << ::std::endl;
        return 1;
    }

    Path filepath(argv[1]);
    BangPreprocessor::Preprocess(filepath);
}
