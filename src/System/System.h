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
            int fillme;
        };
        CmdLineOptions ParseCmdLine(int argc, char **argv);
    }
}
