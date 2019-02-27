#include <System/System.h>
#include <System/Logger.h>

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
                if(!strcmp(o+2, "nosound")) {
                    opt.sound = false;
                }else
                if(!strcmp(o+2, "help")) {
                    std::cout << infotext << std::endl;
                    opt.info = true;
                }else
                if(!strcmp(o+2, "debug")) {
                    opt.debug = true;
                }else
                if(!strcmp(o+2, "fullscreen")) {
                    opt.fullscreen = true;
                }else
                if(!strcmp(o+2, "nointro")) {
                    opt.showintro = false;
                }
                continue;
            }
            /* List of options "-abc" */
            for(int ii = 1; ii < len; ii++) {
                switch(o[ii]) {
                    /* help */
                    case 'h': {
                        std::cout << infotext << std::endl;
                        opt.info = true;
                    }break;
                    case 'f' : {
                        opt.fullscreen = true;
                    }break;
                    case 'd' : {
                        opt.debug = true;
                    }break;
                    default: {
                        Logger::error("SYSTEM") << "Invalid option '-" << o[ii] << "'" << std::endl;
                        opt.valid = false;
                    }
                }
            }
        }else {
            Logger::error("SYSTEM") << "Invalid option '" << o << "'" << std::endl;
            opt.valid = false;
        }
    }

    return opt;
}
