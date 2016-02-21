#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string userBehavioursFilePath = "../res/Assets/userBehavioursFilepathsList.ubfl";

const std::string StageReaderHeaderFilepath = "../src/Domain/Engine/Assets/StageReader.h";
const std::string StageReaderImplFilepath = "../src/Domain/Engine/Assets/StageReader.cpp";

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

void PreprocessStageReaderImpl()
{
    std::ifstream f(StageReaderImplFilepath);
    if(!f.is_open())
    {
        Logger_Error("Error opening file to preprocess '" << StageReaderImplFilepath << "'.");
        exit(-1);
    }

    std::string line;
    while( (line = ReadNextLine(f)).empty() )
    {
        std::ofstream os(StageReaderImplFilepath, std::ios::out | std::ios::app );
        if(!f.is_open())
        {
            Logger_Error("Error opening file to preprocess '" << StageReaderImplFilepath << "'.");
            exit(-1);
        }

        std::vector<std::string> userBehaviourNames;
        GetAllUserBehaviourClassNames(&userBehaviourNames);

        os << "void StageReader::ReadUserBehaviours(const std::string &line, Part **p) {";
        for(std::string bName : userBehaviourNames)
        {
            os << "if(line == <" << bName << ">) { *p = new " << bName << "(); }";
            std::cerr << "if(line == <" << bName << ">) { *p = new " << bName << "(); }" << std::endl;
        }
        os << "}";
    }
}

int main()
{
    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");
    Logger_Log("::::::::::: Bang Preprocessor :::::::::::::");
    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");

    Logger_Log("Running Bang Preprocessor on file '" << StageReaderImplFilepath << "'...");
    PreprocessStageReaderImpl();
    Logger_Log("Successfully run Bang Preprocessor on file '" << StageReaderImplFilepath << "'.");

    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");
    Logger_Log(":::: Finished running Bang Preprocessor :::");
    Logger_Log(":::::::::::::::::::::::::::::::::::::::::::");

    return 0;
}

