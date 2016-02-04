#include "Logger.h"


void Logger::Log(const std::string &log)
{
    std::cerr << log << std::endl;
}

void Logger::Warning(const std::string &warning)
{
    std::cerr << warning << std::endl;
}

void Logger::Error(const std::string &error)
{
    std::cerr << error << std::endl;
}
