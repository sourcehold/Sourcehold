#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>

#include <Config.h>

namespace Sourcehold
{
    namespace System
    {
        struct CmdLineOptions {
            bool info = false;
            bool valid = true;
        };
        CmdLineOptions ParseCmdLine(int argc, char **argv);
    }
}
