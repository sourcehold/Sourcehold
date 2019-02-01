#pragma once

#include <iostream>
#include <fstream>

#include <Config.h>

namespace OpenSH
{
    namespace System
    {
        class Logger
        {
            public:
                Logger();
                ~Logger();

                enum LogType {
                    ERROR,
                    WARNING,
                    MESSAGE,
                    SUCCESS
                };

                static void LogToConsole(std::string msg, LogType type);
                static void LogToFile(std::string msg, LogType type);
        };
    }
}
