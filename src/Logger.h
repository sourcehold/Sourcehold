#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

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

                static std::ostream &log(LogType type, const std::string subsystem);
                static std::ostream &error(const std::string subsystem);
                static std::ostream &warning(const std::string subsystem);
                static std::ostream &message(const std::string subsystem);
                static std::ostream &success(const std::string subsystem);
            private:
            #ifdef OPENSH_UNIX
                static bool const coloredOutput = true;
            #else
                static bool const coloredOutput = false;
            #endif
        };
    }
}
