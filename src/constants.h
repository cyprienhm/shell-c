#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "builtins.h"

extern const int NUM_TOKENS;
extern const int COMMAND_LEN;
extern const int PATH_LEN;
extern const int NUM_COMMANDS;
extern const char *commands[];
extern int (*const commands_functions[])(char **);

#endif