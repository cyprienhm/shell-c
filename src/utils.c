#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "utils.h"

char *find_command_in_path(char *command) {
  char *result = calloc(PATH_LEN, sizeof(char));
  char **path_dirs = get_path_dirs();
  if (path_dirs == NULL)
    return NULL;
  int path_i = 0;

  while (path_dirs[path_i] != NULL) {
    char **path_dir_contents = list_dir(path_dirs[path_i]);
    if (path_dir_contents == NULL) {
      path_i++;
      continue;
    }
    int content_i = 0;
    while (path_dir_contents[content_i] != NULL) {

      if (strcmp(command, path_dir_contents[content_i]) == 0) {
        strcat(result, path_dirs[path_i]);
        strcat(result, "/");
        strcat(result, command);

        return result;
      }
      content_i++;
    }

    path_i++;
  }

  return NULL;
}

char **get_path_dirs() {
  const char *path = getenv("PATH");
  if (path == NULL)
    return NULL;
  return tokenize(path, ':');
}

void print_invalid(char *command) {
  printf("%s: command not found\n", command);
}

char **tokenize(const char *const s, char sep) {
  int last_i = 0;
  int i = 0;
  int tokens_i = 0;
  char **tokens = calloc(NUM_TOKENS, sizeof(char *));

  while (s[i] != '\0') {
    // skip all separators
    while (s[i] != '\0' && s[i] == sep) {
      i++;
      last_i = i;
    }

    if (s[i] == '\0')
      break;

    // advance while in a token
    while (s[i] != '\0' && s[i] != sep) {
      i++;
    }

    int length = i - last_i;
    tokens[tokens_i] = calloc(length + 1, sizeof(char));
    memcpy(tokens[tokens_i], s + last_i, length);
    tokens[tokens_i][length] = '\0';
    tokens_i++;
    last_i = i;
  }

  tokens[tokens_i] = NULL;

  return tokens;
}

char **list_dir(const char *path) {
  const int CONTENT_SIZE = 500;
  int content_buf_size = CONTENT_SIZE;
  char **contents = malloc(sizeof(char *) * content_buf_size);
  struct dirent *dp;
  DIR *dir = opendir(
      path); // Open the directory - dir contains a pointer to manage the dir

  if (dir == NULL) {
    return NULL;
  }

  int i = 0;
  while ((dp = readdir(dir))) {
    contents[i] = malloc(sizeof(char) * PATH_LEN);
    strcpy(contents[i], dp->d_name);
    i++;
    if (i >= content_buf_size) {
      content_buf_size += CONTENT_SIZE;
      contents = realloc(contents, sizeof(char *) * content_buf_size);
    }
  }
  closedir(dir); // close the handle (pointer)
  contents[i] = NULL;
  return contents;
}

int run_cmd(char *executable, char **tokens) {
  char *to_run = calloc(COMMAND_LEN, sizeof(char));
  strcpy(to_run, executable);
  int i = 1;
  while (tokens[i] != NULL) {
    strcat(to_run, " ");
    strcat(to_run, tokens[i]);
    i++;
  }
  system(to_run);
  free(to_run);
  return 0;
}
