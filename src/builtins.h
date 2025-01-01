#ifndef BUILTINS_H
#define BUILTINS_H

int get_builtin_index(char **tokens);

int echo_cmd(char **tokens);
int exit_cmd(char **tokens);
int type_cmd(char **tokens);
int pwd_cmd(char **tokens);
int cd_cmd(char **tokens);

#endif