#include <System/Logger.h>

using namespace Sourcehold::System;

static bool coloredOutput;

Logger::Logger() {

}

Logger::~Logger() {
}

std::ostream &Logger::log(LogType type, const std::string subsystem) {
    std::string msg = "";
    if(coloredOutput) {
        switch(type) {
            case ERROR: msg = "\033[1;31m[ " + subsystem + " ]\033[0m -> "; break;
            case WARNING: msg = "\033[1;33m[ " + subsystem + " ]\033[0m -> "; break;
            case MESSAGE: case SUCCESS: msg = "Info: "; break;
            default: break;
        }
    }else {
        switch(type) {
            case ERROR: msg = "[ " + subsystem + " ] -> "; break;
            case WARNING: msg = "[ " + subsystem + " ] -> "; break;
            case MESSAGE: case SUCCESS: msg = "[ " + subsystem + " ] -> "; break;
            default: break;
        }
    }
    return std::cout << msg;
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

void Logger::SetColorOutput(bool a) {
    coloredOutput = a;
}

bool Logger::GetColorOutput() {
    return coloredOutput;
}
