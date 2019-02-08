#include <System/System.h>

using namespace Sourcehold::System;

static const char infotext[] = {
    "--- Sourcehold version " SOURCEHOLD_VERSION_STRING " ---\n"
    "--- Copyright (C) 2019 Julian Offenhäuser ---\n"
};

CmdLineOptions Sourcehold::System::ParseCmdLine(int argc, char **argv) {
    CmdLineOptions opt;

    for(int i = 1; i < argc; i++) {
        char *o = argv[i];
        int len = std::strlen(o);

        if(len < 2) {
            continue;
        }

        /* Valid option beginning with "-" or "--" */
        if(o[0] == '-') {
            /* Single option "--option" */
            if(o[1] == '-') {
                continue;
            }
            /* List of options "-abc" */
            for(int ii = 1; ii < len; ii++) {
                switch(o[ii]) {
                    /* help */
                    case 'h': {
                        std::cout << infotext << std::endl;
                        exit(EXIT_SUCCESS);
                    }break;
                    default: {
                        std::cout << infotext << "\nInvalid option '-" << o[ii] << "'" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }else {
            std::cout << infotext << "\nInvalid option '" << o << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return opt;
}
