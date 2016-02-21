#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string GeneratedFilesDir = "GeneratedFiles/";

const std::string UserBehaviourElseIfsFilepath = GeneratedFilesDir + "BP_StageReader_cpp_UserBehaviours_elseifs.bp";
const std::string userBehavioursFilePath = "../res/Assets/userBehavioursNamesList.ubnl";


std::string ReadNextLine(std::istream &f)
{
    std::string line;
    do { std::getline(f, line); }
    while( (line.empty() || line.at(0) == '#' || line.at(0) == '\n') && f.peek() != EOF); //Skip all empty/comment lines
    return line;
}


void GetAllUserBehaviourClassNames(std::vector<std::string> *behaviourClassNames)
{
    std::string bName;
    std::ifstream f(userBehavioursFilePath);
    while( (bName = ReadNextLine(f)) != "")
    {
        behaviourClassNames->push_back(bName);
        std::cerr << "Reading behaviour name " << bName << std::endl;
    }
}

void Preprocess_BP_StageReader_cpp_UserBehaviours_elseifs()
{
    std::vector<std::string> userBehaviourNames;
    GetAllUserBehaviourClassNames(&userBehaviourNames);

    std::ofstream os(UserBehaviourElseIfsFilepath, std::ofstream::out);
    if(!os.is_open())
    {
        std::cerr << "Error opening file to preprocess '" << UserBehaviourElseIfsFilepath << "'." << std::endl;
        exit(-1);
    }

    //Add #includes
    for(std::string bName : userBehaviourNames)
    {
        os << "#include \"" << bName << ".h\"" << std::endl;
    }

    os << "#define BANG_PREPROCESSOR_USERBEHAVIOURS_ELSEIFS() \\" << std::endl;
    for(std::string bName : userBehaviourNames)
    {
        os << "if(line == \"<" << bName << ">\")\\" << std::endl;
        os << "{\\" << std::endl;
        os << "     p = new " << bName << "();\\" << std::endl;
        os << "     FileReader::ReadNextLine(f);\\" << std::endl;
        os << "} else ";
    }
    os << "{ }" << std::endl; //To complete the final else...
    os.flush();
    os.close();
}

int main()
{
    std::cerr << std::endl;
    std::cerr << ":::::::::::::::::::::::::::::::::::::::::::" << std::endl;
    std::cerr << "::::::::::: Bang Preprocessor :::::::::::::" << std::endl;
    std::cerr << ":::::::::::::::::::::::::::::::::::::::::::" << std::endl << std::endl;

    std::cerr << "Running Bang Preprocessor on file '" << UserBehaviourElseIfsFilepath << "'..." << std::endl;
    Preprocess_BP_StageReader_cpp_UserBehaviours_elseifs();
    std::cerr << "Successfully run Bang Preprocessor on file '" << UserBehaviourElseIfsFilepath << "'." << std::endl;

    std::cerr << std::endl;
    std::cerr << ":::::::::::::::::::::::::::::::::::::::::::" << std::endl;
    std::cerr << ":::: Finished running Bang Preprocessor :::" << std::endl;
    std::cerr << ":::::::::::::::::::::::::::::::::::::::::::" << std::endl << std::endl;

    return 0;
}

