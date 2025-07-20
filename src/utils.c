#include <dirent.h>
#include <stdbool.h>
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

void flush_tokens(char ***tokens, const char *const s, int *tokens_i,
                  int last_i, int length,
                  enum tokenization_state *current_state) {
  (*tokens)[*tokens_i] = calloc(length + 1, sizeof(char));
  if ((*tokens)[*tokens_i] == NULL)
    exit(EXIT_FAILURE);
  memcpy((*tokens)[*tokens_i], s + last_i, length);
  (*tokens)[*tokens_i][length] = '\0';
  (*tokens_i)++;
  *current_state = SKIPPING;
}

char **tokenize(const char *const s, char sep) {
  int last_i = 0;
  int i = 0;
  int tokens_i = 0;
  size_t capacity = NUM_TOKENS;
  char **tokens = calloc(capacity, sizeof(char *));
  if (tokens == NULL)
    exit(EXIT_FAILURE);

  int quote_start_index = 0;

  enum tokenization_state current_state = SKIPPING;
  while (s[i] != '\0') {
    if (tokens_i >= capacity) {
      capacity *= 2;
      char **new_tokens = realloc(tokens, sizeof(char *) * capacity);
      if (new_tokens == NULL)
        exit(EXIT_FAILURE);
      tokens = new_tokens;
    }
    char c = s[i];
    switch (current_state) {
    case SKIPPING:
      if (c == sep) {
        i++;
        last_i = i;
      } else if (c == '"') {
        quote_start_index = i;
        i++;
        last_i = i;
        current_state = QUOTED;
      } else {
        current_state = TOKEN;
      }
      break;
    case TOKEN:
      if (c == sep) {
        flush_tokens(&tokens, s, &tokens_i, last_i, i - last_i, &current_state);
      } else {
        i++;
      }
      break;
    case QUOTED:
      if (c == '"') {
        flush_tokens(&tokens, s, &tokens_i, last_i, i - last_i, &current_state);
        i++;
      } else {
        i++;
      }
      break;
    }
  }

  if (current_state == TOKEN) {
    flush_tokens(&tokens, s, &tokens_i, last_i, i - last_i, &current_state);
  } else if (current_state == QUOTED) {
    fprintf(stderr, "Error: unterminated quote.\n");
    return NULL;
  }

  if (tokens_i == 0)
    return NULL;

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
