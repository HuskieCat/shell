#include <stdio.h>
#include <sys/wait.h> //wait
#include <unistd.h> //fork, execlp
#include <stdlib.h>

#define LSH_RL_BUFSIZE 1024

void lsh_loop();
char *lsh_read_line();

int main(int argc, char** argv) 
{ 
  //char *envp[] = {(char*) "PATH=/bin", 0};

  lsh_loop();

  return 0;
}

void lsh_loop()
{
  char *line;
  char **args;
  int status;

  while(status)
  {
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  }
}

char *lsh_read_line()
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if(!buffer)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(1);
  }

  while(1)
  {
    c = getchar();

    if(c == EOF || c == '\n')
    {
      buffer[position] = '\0';
      return buffer;
    }
    else
    {
      buffer[position] = c;
    }
  }
}