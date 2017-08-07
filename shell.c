#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define JSH_RL_BUFSIZE 1024
#define JSH_TOK_BUFSIZE 64
#define JSH_TOK_DELIM " \t\r\n\a"
#define JSH_PATH_BUF 256


int jsh_cd(char **args);
int jsh_exit(char **args);

int (*builtin_func[]) (char **) = {
  &jsh_cd,
  &jsh_exit
};

int jsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "jsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("jsh");
    }
  }
  return 1;
}

int jsh_exit(char **args)
{
  return 0;
}

int jsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Hijo
    if (execvp(args[0], args) == -1) {
      perror("jsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("jsh");
  } else {
    // Padre
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int jsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return jsh_launch(args);
}



char *jsh_read_line(void)
{
  int bufsize = JSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "jsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Lee el caracter
    c = getchar();

    // Si llegamos al final de la linea añadimos un NULL
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // Realojamos el buffer si lo llenamos
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "jsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **jsh_parser(char *line)
{
  int bufsize = JSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "jsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, JSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += JSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "jsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, JSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

void jsh_loop(void)
{
  char *line;
  char **args;
  int status;
  char path[JSH_PATH_BUF]

  do {
	  
    getcwd(path, 255);
    strcat(path, "/");
    printf("%s >", the_path);
    line = jsh_read_line();
    args = jsh_parser(line);
    status = jsh_execute(args);

    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{
 jsh_loop();

  return EXIT_SUCCESS;
}