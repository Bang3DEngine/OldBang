#include "Logger.h"


void Logger::Log(const std::string &log)
{
    std::cout << log << std::endl;
}

void Logger::Warning(const std::string &warning)
{
    std::cout << warning << std::endl;
}

void Logger::Error(const std::string &error)
{
    std::cerr << error << std::endl;
}
