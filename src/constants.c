#include "builtins.h"

const int NUM_TOKENS = 128;
const int COMMAND_LEN = 1000;
const int PATH_LEN = 1000;
const int NUM_COMMANDS = 5;
const char *commands[] = {"exit", "echo", "type", "pwd", "cd"};
int (*const commands_functions[])(char **) = {&exit_cmd, &echo_cmd, &type_cmd, &pwd_cmd, &cd_cmd};