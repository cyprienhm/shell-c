#ifndef UTILS_H
#define UTILS_H

enum tokenization_state {
  SKIPPING,
  TOKEN,
  QUOTED,
};

char *find_command_in_path(char *command);
char **get_path_dirs();
void print_invalid(char *command);
void flush_tokens(char ***tokens, const char *const s, int *tokens_i,
                  int last_i, int length,
                  enum tokenization_state *current_state);
char **tokenize(const char *const s, char sep);
char **list_dir(const char *path);
int run_cmd(char *input);

#endif
