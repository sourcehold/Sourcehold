#include <Logger.h>

using namespace OpenSH::System;

Logger::Logger() {

}

Logger::~Logger() {

}

std::ostream &Logger::log(LogType type, const std::string subsystem) {
    std::string msg = "";
    switch(type) {
        case ERROR: msg = "Error: "; break;
        case WARNING: msg = "Warning: "; break;
        case MESSAGE: case SUCCESS: msg = "Info: "; break;
        default: break;
    }
    
    return std::cout << "[" << subsystem << "] " << msg;
}

std::ostream &Logger::error(const std::string subsystem) {
    return log(ERROR, subsystem);
}

std::ostream &Logger::warning(const std::string subsystem) {
    return log(WARNING, subsystem);
}

std::ostream &Logger::message(const std::string subsystem) {
    return log(MESSAGE, subsystem);
}

std::ostream &Logger::success(const std::string subsystem) {
    return log(SUCCESS, subsystem);
}
