#include <stdio.h>
#include <sys/wait.h> //wait
#include <unistd.h> //fork, execlp
#include <stdlib.h>
#include <string.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

void lsh_loop();
char *lsh_read_line();
char **lsh_split_line(char *line);
int lsh_launch(char **args);

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_num_builtins();
int lsh_execute(char **args);

int main(int argc, char** argv) 
{
  printf(" /\\_/\\\n( o.o )\n > ^ <\n");
  lsh_loop();
  return 0;
}

void lsh_loop()
{
  char *line;
  char **args;
  int status = 1;

  while(status)
  {
    printf("Tortoiseshell [Bradley Henderson]: ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  }
}

char *lsh_read_line()
{
  char *line = NULL;
  ssize_t bufsize = 0;

  if(getline(&line, &bufsize, stdin) == -1)
  {
    if(feof(stdin))
      exit(0);
    else
    {
      perror("readline");
      exit(1);
    }
  }

  return line;
}

char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if(!tokens)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(1);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while(token != NULL)
  {
    tokens[position] = token;
    position++;

    if(position >= bufsize)
    {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if(!tokens)
      {
        fprintf(stderr, "lsh: allocation error\n");
        exit(1);
      }
    }
    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if(pid == 0)
  {
    if(execvp(args[0], args) == -1)
      perror("lsh");
    exit(1);
  }
  else if(pid < 0)
    perror("lsh");
  else
  {
    while(!WIFEXITED(status) && !WIFSIGNALED(status))
      wpid = waitpid(pid, &status, WUNTRACED);
  }
  return 1;
}

char *builtin_str[] = {"cd", "help", "exit"};

int(*builtin_func[]) (char**) = {&lsh_cd, &lsh_help, &lsh_exit};

int lsh_num_builtins()
{
  return sizeof(builtin_str) / sizeof(char *);
}

int lsh_cd(char **args)
{
  if(args[1] == NULL)
  {
    fprintf(stderr, "lsh: expected argument tp \"cd\"\n");
  }
  else
  {
    if(chdir(args[1]) != 0)
    {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_help(char **args)
{
  int i;
  printf("Stephen Brennan's LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for(int i = 0; i < lsh_num_builtins(); i++)
    printf(" %s\n", builtin_str[i]);

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int lsh_exit(char **args)
{
  return 0;
}

int lsh_execute(char **args)
{
  int i;
  if(args[0] == NULL)
    return 1;

  for(i = 0; i < lsh_num_builtins(); i++)
      if(strcmp(args[0], builtin_str[i]) == 0)
        return (*builtin_func[i]) (args);

  return lsh_launch(args);
}