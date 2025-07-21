#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "constants.h"
#include "utils.h"
#include <stdbool.h>

volatile sig_atomic_t sigint = 0;

void handle_sigint(int signo) {
  sigint = 1;
  char buf[4] = {'\n', SHELL_PS1, ' ', '\0'};
  write(STDOUT_FILENO, buf, 3);
}

int main() {
  setbuf(stdout, NULL);
  signal(SIGINT, handle_sigint);

  while (true) {

    if (sigint) {
      sigint = 0;
      continue;
    }
    printf("%c ", SHELL_PS1);

    char *input = NULL;
    size_t size = 0;
    ssize_t nread = getline(&input, &size, stdin);
    if (nread == -1) {
      free(input);
      if (feof(stdin))
        exit(EXIT_SUCCESS);
      else if (ferror(stdin)) {
        clearerr(stdin);
        continue;
      } else
        exit(EXIT_FAILURE);
    }

    if (input[nread - 1] == '\n') {
      input[nread - 1] = '\0';
    }

    if (nread == 1)
      continue;

    char **tokens = tokenize(input, ' ');
    if (tokens == NULL) {
      free(input);
      continue;
    }

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
    for (int j = 0; tokens[j] != NULL; j++)
      free(tokens[j]);
    free(input);
  }
  return 0;
}
