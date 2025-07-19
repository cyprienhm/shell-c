#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "constants.h"
#include "utils.h"

int get_builtin_index(char **tokens) {
  char *first_token = tokens[0];
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (strcmp(first_token, commands[i]) == 0)
      return i;
  }

  return -1;
}

int cd_cmd(char **tokens) {
  char *second_token = tokens[1];
  if (second_token == NULL)
    return 0;
  if (chdir(tokens[1]) != 0)
    printf("cd: %s: No such file or directory\n", tokens[1]);
  return 0;
}
int echo_cmd(char **tokens) {
  int i = 1;
  char *token = tokens[i];

  while (token != NULL) {

    printf("%s ", token);
    token = tokens[++i];
  }
  printf("\n");
  return 0;
}

int exit_cmd(char **tokens) { exit(0); }

int type_cmd(char **tokens) {
  char *second_token = tokens[1];
  if (second_token == NULL)
    return 0;
  int builtin_index = get_builtin_index(tokens + 1);
  if (builtin_index != -1) {
    printf("%s is a shell builtin\n", tokens[1]);
  } else {
    char *found = find_command_in_path(tokens[1]);
    if (found == NULL) {
      printf("%s: not found\n", tokens[1]);
    } else {
      printf("%s is %s\n", tokens[1], found);
    }
  }
  return 0;
}

int pwd_cmd(char **tokens) {
  int size = PATH_LEN;
  char *buf = malloc(sizeof(char) * size);
  char *ptr = NULL;
  while ((ptr = getcwd(buf, size)) == NULL) {
    size *= 2;
    buf = realloc(buf, sizeof(char) * size);
  }
  printf("%s\n", buf);
  free(buf);
  return 0;
}