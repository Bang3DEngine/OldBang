#include "Logger.h"


void Logger::Log(const std::string &log)
{
    std::cerr << "[   LOG   ]: " << log << std::endl;
}

void Logger::Warning(const std::string &warning)
{
    std::cerr << "[ WARNING ]: " << warning << std::endl;
}

void Logger::Error(const std::string &error)
{
    std::cerr << "[  ERROR  ]: " <<  error << std::endl;
}
