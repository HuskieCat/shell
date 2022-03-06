#include <stdio.h>

void type_prompt();
void read_command(char, char);

int main(void) 
{
  char cmd[100], command[100], *parameters[20];

  char *envp[] = {(char*) "PATH=/bin", 0};
  while(1)
  {
    type_prompt();
    read_command(command, parameters);
    if(fork() == 0) //Child
    {
      strcpy(cmd, "/bin/");
      strcat(cmd, command);
      execvp(cmd, parameters, envp);
    } 
    else //Parent
    {
      wait();
    }
  }
  return 0;
}

void type_prompt()
{
  static int first_time = 1;
  if(first_time)
  {
    const char* clear_screen = " \e[1;1H\e[2J";
    write(1, clear_screen, 12);
    first_time = 0;
  }

  printf("Cat-Shell");
}

void read_command(char chmd[], char* par[])
{
  char line[1024];
  int count = 0, i = 0, j = 0;
  char *array[100], *pch;

  for(;;)
  {
    int c = fgetc(stdin);
    line[count++] = (char) c;
    if(c == '\n')
      break;
  }

  if(count == 1)
    return;

  pch = strtok(line, " \n");

  while (pch != NULL)
  {
    array[i++] = strdup (pch);
    pch = strtok(NULL, " \n");
  }

  strcpy(cmd, array[0]);

  for(int j = 0; j < i; j++)
    par[j] = array[j];
  par[i] = NULL;
}