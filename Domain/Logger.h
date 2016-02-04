#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

class Logger
{
private:
    Logger();

public:
    static void Log(const std::string& log);
    static void Warning(const std::string& warning);
    static void Error(const std::string& error);
};

#endif // LOGGER_H
