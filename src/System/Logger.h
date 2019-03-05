#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <Config.h>

namespace Sourcehold
{
    namespace System
    {
        /*
         * Logger class, redirects logging messages to a file,
         * stdout or any ostream provider (TODO!)
         */
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
            #ifdef SOURCEHOLD_UNIX
                static bool const coloredOutput = true;
            #else
                static bool const coloredOutput = false;
            #endif
        };
    }
}
