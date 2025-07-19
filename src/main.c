#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "constants.h"
#include "utils.h"
#include <stdbool.h>

int main() {
  setbuf(stdout, NULL);

  while (true) {
    printf("$ ");
    // Wait for user input
    char input[COMMAND_LEN];
    fgets(input, COMMAND_LEN, stdin);

    size_t input_len = strlen(input);
    input[input_len - 1] = '\0';
    input_len--;

    if (input_len == 0)
      continue;

    char **tokens = tokenize(input, ' ');
    if (tokens == NULL)
      continue;

    int builtin_index = get_builtin_index(tokens);
    if (builtin_index == -1) {

      char *found = find_command_in_path(tokens[0]);

      if (found == NULL) {
        print_invalid(input);
      } else {
        run_cmd(found, tokens);
      }
    } else {
      commands_functions[builtin_index](tokens);
    }
  }
  return 0;
}
