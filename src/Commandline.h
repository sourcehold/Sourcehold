#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_
#include "GameOptions.h"
// This may exit the application on:
// --help
// --resolutions
GameOptions getGameOptions(int argc, char** argv);
#endif  // COMMANDLINE_H_
