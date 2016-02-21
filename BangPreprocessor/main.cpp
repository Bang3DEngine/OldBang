#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string GeneratedFilesDir = "GeneratedFiles";

const std::string userBehavioursFilePath = "../res/Assets/userBehavioursFilepathsList.ubfl";

std::string ReadNextLine(std::istream &f)
{
    std::string line;
    do { std::getline(f, line); }
    while( (line.empty() || line.at(0) == '#' || line.at(0) == '\n') && f.peek() != EOF); //Skip all empty/comment lines
    return line;
}


void GetAllUserBehaviourClassNames(std::vector<std::string> *behaviourClassNames)
{
    std::string readFilepath;
    std::ifstream f(userBehavioursFilePath);
    while( (readFilepath = ReadNextLine(f)) != "")
    {
        int slashPos = readFilepath.rfind("/");
        std::string behaviourName = readFilepath.substr(slashPos, readFilepath.rfind(".", slashPos) - slashPos);
        behaviourClassNames->push_back(behaviourName);
    }
}

void Preprocess_BP_StageReader_cpp_UserBehaviours_elseifs()
{
    const std::string filepath = "BP_StageReader_cpp_UserBehaviours_elseifs.bp";
    std::ifstream f(filepath);
    if(!f.is_open())
    {
        Logger_Error("Error opening file to preprocess '" << filepath << "'.");
        exit(-1);
    }

    std::vector<std::string> userBehaviourNames;
    GetAllUserBehaviourClassNames(&userBehaviourNames);

    //Add #includes
    for(std::string bName : userBehaviourNames)
    {
        os << "#include \"" << bName << ".h\"";
    }

    os << "#define BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS \\" << std::endl;
    for(std::string bName : userBehaviourNames)
    {
        os << "if(line == <" << bName << ">)\\" << std::endl;
        os << "{\\" << std::endl;
        os << "     *p = new " << bName << "();\\" << std::endl;
        os << "     FileReader::ReadNextLine(f);\\" << std::endl;
        os << "} else ";
    }
    os << "{ }" << std::endl; //To complete the final else...
}

int main()
{
    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");
    Logger_Log("::::::::::: Bang Preprocessor :::::::::::::");
    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");

    Logger_Log("Running Bang Preprocessor on file '" << StageReaderImplFilepath << "'...");
    Preprocess_BP_StageReader_cpp_UserBehaviours_elseifs();
    Logger_Log("Successfully run Bang Preprocessor on file '" << StageReaderImplFilepath << "'.");

    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");
    Logger_Log(":::: Finished running Bang Preprocessor :::");
    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");

    return 0;
}

