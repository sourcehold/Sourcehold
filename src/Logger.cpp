#include <Logger.h>

using namespace OpenSH::System;

Logger::Logger() {

}

Logger::~Logger() {

}

void Logger::LogToConsole(std::string msg, LogType type) {
    std::cout << msg << std::endl;
}

void Logger::LogToFile(std::string msg, LogType type) {

}
