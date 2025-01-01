#ifndef UTILS_H
#define UTILS_H

char *find_command_in_path(char *command);
char **get_path_dirs();
void print_invalid(char *command);
char **tokenize(const char *const s, char sep);
char **list_dir(const char *path);
int run_cmd(char *executable, char **tokens);

#endif